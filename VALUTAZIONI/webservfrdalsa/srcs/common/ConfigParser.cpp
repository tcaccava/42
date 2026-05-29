#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "StringUtils.hpp"

// Costruttore di default
ConfigParser::ConfigParser() {}

// Restituisce la lista dei server configurati
const std::vector<ServerConfig>& ConfigParser::getServers() const {
	return servers;
}

// Suddivide il file di configurazione in token
std::vector<std::string> ConfigParser::tokenize(std::istream& file) {
	std::vector<std::string> tokens;
	std::string token;
	char ch;
	while (file.get(ch)) {
		if (ch == '#') {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
			// Skip until end of line for comments
			while (file.get(ch) && ch != '\n')
				;
			if (ch == '\n') {
				if (!token.empty()) {
					tokens.push_back(token);
					token.clear();
				}
			}
		} else if (isspace(ch)) {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		} else if (ch == '{' || ch == '}' || ch == ';') {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
			tokens.push_back(std::string(1, ch));
		} else {
			token += ch;
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
	return tokens;
}

// Analizza il blocco "location" all'interno di un server
bool ConfigParser::parseLocationBlock(std::vector<std::string>& tokens, size_t& i, ServerConfig& server) {
	if (i >= tokens.size()) return false;

	LocationConfig loc;
	loc.path = StringUtils::trim(tokens[i++]);
	if (i >= tokens.size() || tokens[i++] != "{") return false;
	while (i + 2 < tokens.size() && tokens[i] != "}") {
		std::string key = tokens[i++];
		std::string value;
		while (i < tokens.size() && tokens[i] != ";") {
			if (!value.empty()) value += " ";
			value += tokens[i++];
		}
		if (i >= tokens.size() || tokens[i++] != ";") return false;

		if (key == "root")
			loc.root = StringUtils::trim(value);
		else if (key == "index")
			loc.index = StringUtils::trim(value);
		else if (key == "autoindex")
			loc.autoindex = (StringUtils::trim(value) == "on");
		else if (key == "allowed_methods")
			loc.allowed_methods = StringUtils::trim(value);
		else if (key == "cgi_extension")
			loc.cgi_extension = StringUtils::trim(value);
		else if (key == "cgi_path")
			loc.cgi_path = StringUtils::trim(value);
		else if (key == "upload_store")
			loc.upload_store = StringUtils::trim(value);
		else if (key == "redirect")
			loc.redirect = StringUtils::trim(value);
		else {
			std::cerr << "Errore: direttiva non riconosciuta '" << key
				  << "' nel blocco location" << std::endl;
			return false;
		}
	}

	if (i < tokens.size() && tokens[i] == "}") ++i;
	server.locations.push_back(loc);
	return true;
}

// Analizza un blocco "server" e popola la configurazione
bool ConfigParser::parseServerBlock(std::vector<std::string>& tokens, size_t& i) {
	ServerConfig server;
	if (i >= tokens.size() || tokens[i++] != "{") return false;

	while (i < tokens.size() && tokens[i] != "}") {
		if (tokens[i] == "location") {
			++i;
			if (!parseLocationBlock(tokens, i, server))
				return false;
		} else if (i + 2 < tokens.size()) {
			std::string key = tokens[i++];
			std::string value;
			while (i < tokens.size() && tokens[i] != ";") {
				if (!value.empty()) value += " ";
				value += tokens[i++];
			}
			if (i >= tokens.size() || tokens[i++] != ";") return false;

			if (key == "listen")
				server.listen = StringUtils::trim(value);
			else if (key == "server_name")
				server.server_name = StringUtils::trim(value);
			else if (key == "root")
				server.root = StringUtils::trim(value);
			else if (key == "error_page") {
				std::istringstream iss(StringUtils::trim(value));
				std::vector<std::string> parts;
				std::string part;
				while (iss >> part)
					parts.push_back(part);
				if (parts.size() >= 2) {
					std::string path = parts.back();
					for (size_t j = 0; j < parts.size() - 1; ++j) {
						int code = std::atoi(parts[j].c_str());
						server.error_pages[code] = path;
					}
				}
			}
			else if (key == "index") {
				// direttiva index a livello server non utilizzata
			}
			else if (key == "client_max_body_size")
				server.client_max_body_size = std::atoi(StringUtils::trim(value).c_str());
			else if (key == "cgi_timeout")
				server.cgi_timeout = std::atoi(StringUtils::trim(value).c_str());
			else {
				std::cerr << "Errore: direttiva non riconosciuta '" << key
					  << "' nel blocco server" << std::endl;
				return false;
			}
		} else {
			break;
		}
	}

	if (i < tokens.size() && tokens[i] == "}") ++i;
	servers.push_back(server);
	return true;

}

// Carica e analizza il file di configurazione
bool ConfigParser::parse(const std::string& filepath) {
	std::ifstream file(filepath.c_str());
	if (!file.is_open()) {
		std::cerr << "Errore: impossibile aprire il file " << filepath << std::endl;
		return false;
	}

	std::vector<std::string> tokens = tokenize(file);

	size_t i = 0;
	while (i < tokens.size()) {
		if (tokens[i] == "server") {
			++i;
			if (!parseServerBlock(tokens, i))
				return false;
		} else {
			std::cerr << "Errore: direttiva non riconosciuta '" << tokens[i]
				  << "'" << std::endl;
			return false;
		}
	}

	return true;
}
