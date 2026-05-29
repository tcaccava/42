#include "Parser.hpp"
#include "Exception.hpp"

Parser::Parser(const std::string &filename) : filename(filename) {
    try {
        std::vector<std::string> blocks = extract_server_blocks(filename);
        std::cout << "🔍 Trovati " << blocks.size() << " blocchi server" << std::endl;
        
        for (size_t i = 0; i < blocks.size(); ++i) {
            std::cout << "📦 Processando server block #" << i + 1 << std::endl;
            ServerConfig server = generate_server_instance(blocks[i]);
            
            if (!validate_server_config(server)) {
                std::cerr << "❌ Server #" << i + 1 << " non valido: parametri obbligatori mancanti" << std::endl;
                continue;
            }
            
            servers.push_back(server);
            std::cout << "✅ Server #" << i + 1 << " aggiunto. Porta: " << server.getPort() << std::endl;
        }
        std::cout << "🎯 Totale server parsati: " << servers.size() << std::endl;
        
        if (servers.empty()) {
            throw ConfigurationException("Nessun server valido trovato nella configurazione");
        }
        
    } catch (const ConfigurationException &e) {
        std::cerr << "❌ Configuration error: " << e.what() << std::endl;
        throw;
    } catch (const HttpException &e) {
        std::cerr << "❌ HTTP error during parsing: " << e.what() << std::endl;
        throw;
    }
}

Parser::~Parser() {}

bool Parser::validate_server_config(const ServerConfig &server) {
    if (server.getPort() <= 0 || server.getPort() > 65535) {
        std::cerr << "❌ Porta non valida: " << server.getPort() << std::endl;
        return false;
    }
    if (server.getServerName().empty()) {
        std::cerr << "❌ server_name mancante" << std::endl;
        return false;
    }
    if (server.getRoot().empty()) {
        std::cerr << "❌ root mancante o vuota" << std::endl;
        return false;
    }
    
    std::string rootPath = server.getRoot();
    struct stat statbuf;
    
    if (rootPath[0] == '/') {
        rootPath = "." + rootPath;
    }
    
    if (stat(rootPath.c_str(), &statbuf) != 0) {
        std::cerr << "❌ Directory root non esiste: " << rootPath << " (configurato come: " << server.getRoot() << ")" << std::endl;
        return false;
    }
	
    if (!S_ISDIR(statbuf.st_mode)) {
        std::cerr << "❌ Root non è una directory: " << rootPath << std::endl;
        return false;
    }
    
    return true;
}

std::vector<std::string> Parser::extract_server_blocks(const std::string &filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw ConfigurationException("Cannot open file: " + filename);

	std::vector<std::string> server_blocks;
	std::string line;
	std::ostringstream current_block;
	bool in_server = false;
	int brace_depth = 0;

	while (std::getline(file, line)) {
		// Rimuovi commenti da #
		size_t comment = line.find('#');
		if (comment != std::string::npos)
			line = line.substr(0, comment);

		// Trim iniziale
		std::string::size_type start = line.find_first_not_of(" \t");
		if (start != std::string::npos)
			line = line.substr(start);
		else
			line = "";

		if (line.empty())
			continue;

		if (!in_server && line.find("server") == 0 && line.find("{") != std::string::npos) {
			in_server = true;
			brace_depth = 1;
			current_block.str("");
			current_block.clear();
			current_block << line << "\n";
			continue;
		}

		if (in_server) {
			current_block << line << "\n";
			// Conta graffe
			for (size_t i = 0; i < line.size(); ++i) {
				if (line[i] == '{') brace_depth++;
				else if (line[i] == '}') brace_depth--;
			}

			if (brace_depth == 0) {
				server_blocks.push_back(current_block.str());
				in_server = false;
			}
		}
	}

	if (in_server)
		throw ConfigurationException("Unterminated server block in config file");

	return server_blocks;
}

std::vector<std::string> Parser::extract_location_blocks(const std::string &block) {
	std::vector<std::string> loc_blocks;

	std::istringstream ss(block);
	std::string line;
	bool in_location = false;
	int brace_depth = 0;
	std::ostringstream current_loc;

	while (std::getline(ss, line)) {
		// Rimuovi commenti
		size_t comment = line.find('#');
		if (comment != std::string::npos)
			line = line.substr(0, comment);

		std::string::size_type start = line.find_first_not_of(" \t");
		if (start != std::string::npos)
			line = line.substr(start);
		else
			line = "";

		if (line.empty())
			continue;

		if (!in_location && line.find("location") == 0 && line.find("{") != std::string::npos) {
			in_location = true;
			brace_depth = 1;
			current_loc.str("");
			current_loc.clear();
			current_loc << line << "\n";
			continue;
		}

		if (in_location) {
			current_loc << line << "\n";
			for (size_t i = 0; i < line.size(); ++i) {
				if (line[i] == '{') brace_depth++;
				else if (line[i] == '}') brace_depth--;
			}

			if (brace_depth == 0) {
				loc_blocks.push_back(current_loc.str());
				in_location = false;
			}
		}
	}

	if (in_location)
		throw ConfigurationException("Unterminated location block");

	return loc_blocks;
}

static std::string strip_semicolon(const std::string& s) {
    if (!s.empty() && s[s.size() - 1] == ';')
        return s.substr(0, s.size() - 1);
    return s;
}

ServerConfig Parser::generate_server_instance(const std::string &block) {
	ServerConfig server;

	std::istringstream ss(block);
	std::string line;
	while (std::getline(ss, line)) {
		// Strip comments
		size_t comment = line.find('#');
		if (comment != std::string::npos)
			line = line.substr(0, comment);

		// Trim
		std::string::size_type start = line.find_first_not_of(" \t");
		if (start != std::string::npos)
			line = line.substr(start);
		else
			continue;

		if (line.empty())
			continue;

		// Token parsing
		std::istringstream linestream(line);
		std::string token;
		linestream >> token;

		if (token == "server_name") {
			std::string name;
			while (linestream >> name) {
				server.addServerName(strip_semicolon(name));
			}
		} else if (token == "listen") {
			int port;
			linestream >> port;
			server.setPort(port);
		} else if (token == "root") {
			std::string root;
			linestream >> root;
			server.setRoot(strip_semicolon(root));
		} else if (token == "autoindex") {
			std::string val;
			linestream >> val;
			server.setAutoIndex(strip_semicolon(val) == "on");
		} else if (token == "host") {
			std::string host;
			while (linestream >> host) {
				server.setHost(strip_semicolon(host));
			}
		} else if (token == "allow") {
			std::string method;
			std::vector<std::string> methods;
			while (linestream >> method) {
				methods.push_back(strip_semicolon(method));
			}
			server.setAllowedMethods(methods);
		} else if (token == "client_max_body_size") {
			int size;
			linestream >> size;
			server.setClientMaxBodySize(size);
		} else if (token == "error_page") {
			int code;
			std::string url;
			linestream >> code >> url;
			server.addErrorPage(code, strip_semicolon(url));
		} else if (token == "cgi_extension") {
			std::string ext;
			std::vector<std::string> exts;
			while (linestream >> ext) {
				exts.push_back(strip_semicolon(ext));
			}
			server.setCgiExtensions(exts);
		} else if (token == "location") {
			// il blocco location lo estraiamo a parte
			std::string rest_of_line;
			std::getline(linestream, rest_of_line);

			// ricostruisci il blocco location a partire da "location /path { ... }"
			// Estrai location blocks dal blocco server:
			std::vector<std::string> loc_blocks = extract_location_blocks(block);
			std::vector<LocationConfig> locs;
			for (size_t i = 0; i < loc_blocks.size(); ++i) {
				LocationConfig loc = generate_location_instance(loc_blocks[i], server);
				locs.push_back(loc);
			}
			server.setLocations(locs);
			break;
		}
	}

	return server;
}

LocationConfig Parser::generate_location_instance(const std::string &block, const ServerConfig &server) {
	LocationConfig loc;

	// Eredita dal blocco server
	loc.setRoot(server.getRoot());
	loc.setAllowedMethods(server.getAllowedMethods());
	loc.setAutoIndex(server.getAutoIndex());
	loc.setErrorPages(server.getErrorPages());
	loc.setCgiExtensions(server.getCgiExtensions());

	std::string path = extract_location_path(block);
	loc.setPath(path);

	std::istringstream ss(block);
	std::string line;
	while (std::getline(ss, line)) {
		size_t comment = line.find('#');
		if (comment != std::string::npos)
			line = line.substr(0, comment);

		std::string::size_type start = line.find_first_not_of(" \t");
		if (start != std::string::npos)
			line = line.substr(start);
		else
			continue;

		if (line.empty())
			continue;

		std::istringstream linestream(line);
		std::string token;
		linestream >> token;

		if (token == "location") {
			continue;
		} else if (token == "root") {
			std::string root;
			linestream >> root;
			if (root[root.size() - 1] == ';')
					root = root.substr(0, root.size() - 1);
			loc.setRoot(root);
		} else if (token == "index") {
			std::string index;
			std::vector<std::string> indexes;
			while (linestream >> index) {
				indexes.push_back(strip_semicolon(index));
			}
			loc.setIndexPages(indexes);
		} else if (token == "autoindex") {
			std::string val;
			linestream >> val;
			loc.setAutoIndex(strip_semicolon(val) == "on");
		} else if (token == "allow") {
			std::string method;
			std::vector<std::string> methods;
			while (linestream >> method) {
				methods.push_back(strip_semicolon(method));
			}
			loc.setAllowedMethods(methods);
		} else if (token == "cgi_extension") {
			std::string ext;
			std::vector<std::string> exts;
			while (linestream >> ext) {
				exts.push_back(strip_semicolon(ext));
			}
			loc.setCgiExtensions(exts);
		} else if (token == "upload_dir") {
			std::string dir;
			linestream >> dir;
			loc.setUploadDir(strip_semicolon(dir));
		} else if (token == "return") {
			int code;
			std::string url;
			linestream >> code >> url;
			loc.setRedirectCode(code);
			loc.setRedirectUrl(strip_semicolon(url));
		}
	}

	return loc;
}

std::string Parser::extract_location_path(const std::string &block) {
	std::istringstream iss(block);
	std::string line;

	while (std::getline(iss, line)) {
		std::istringstream line_stream(line);
		std::string keyword;
		line_stream >> keyword;

		if (keyword == "location") {
			std::string path;
			line_stream >> path;
			return path;
		}
	}
	return "";
}

const std::vector<ServerConfig> &Parser::getServers() const {
	return servers;
}
