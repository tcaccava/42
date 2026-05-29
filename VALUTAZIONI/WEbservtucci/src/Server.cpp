#include "Server.hpp"

bool	runner = true;

Server::Server(const char *filename) {
	std::cout << "🛠️  Server: parsing config..." << std::endl;

	try {
		Parser parser(filename);
		this->config = parser.getServers();
        if (!config.empty()) {
            initAllServers();
            initClients();
            mainLoop();
        } else {
            std::cerr << "❌ Error: No valid 'server' blocks found in the configuration file.\n"
                    << "   → Make sure the config file is not empty and contains at least one 'server { ... }' block.\n"
                    << "   → File: " << filename << std::endl;
        }
	} catch (const ConfigurationException& e) {
		std::cerr << "❌ Configuration error: " << e.what() << std::endl;
		throw;  // rilancia l'eccezione per farla gestire a chi ha chiamato
	} catch (const SocketException& e) {
		std::cerr << "❌ Socket error: " << e.what() << std::endl;
		throw;
	} catch (const ServerErrorException& e) {
		std::cerr << "❌ Server error: " << e.what() << std::endl;
		throw;
	} catch (const HttpException& e) {
		std::cerr << "❌ HTTP error: " << e.what() << std::endl;
		throw;
	}
}

Server::Server(const Server &toCopy) {
	*this = toCopy;
}

Server::~Server() {
}

Server& Server::operator=(const Server &other) {
	if (this != &other) {
		this->config = other.config;
		this->clients = other.clients;
		this->serverSockets = other.serverSockets;
		this->socketToConfigIndex = other.socketToConfigIndex;
		this->clientToConfigIndex = other.clientToConfigIndex;
	}
	return *this;
}

void Server::initAllServers() {
	
	serverSockets.clear();
	socketToConfigIndex.clear();
	
	// Pre-alloca lo spazio per evitare rilocazioni che causerebbero distruzione temporanea
	serverSockets.reserve(config.size());
	
	for (size_t i = 0; i < config.size(); ++i) {
		int port = config[i].getPort();
		std::cout << "🚀 Server [" << i << "] starting on port " << port << " !!!" << std::endl;
		
		// Crea direttamente nel vettore per evitare copie
		serverSockets.push_back(Socket());
		Socket& serverSocket = serverSockets.back();
		
		serverSocket.create();
		serverSocket.bind(port);
		serverSocket.listen();
		setNonBlocking(serverSocket.getFd());
		
		// Crea la mappatura
		socketToConfigIndex[serverSocket.getFd()] = i;
	}
	
	std::cout << "✅ All servers initialized successfully!" << std::endl;
}

void Server::initClients() {
	clients.clear();
	for (int i = 0; i < MAX_CLIENTS; i++) {
		Socket s;
		s.setFd(-1);
		clients.push_back(s);
	}
}

const ServerConfig& Server::getConfigForClient(int client_fd) {
	std::map<int, size_t>::iterator it = clientToConfigIndex.find(client_fd);
	if (it != clientToConfigIndex.end()) {
		return config[it->second];
	}
	// Fallback al primo server se non trovato (non dovrebbe mai succedere)
	std::cerr << "⚠️  Warning: Config not found for client " << client_fd << ", using default" << std::endl;
	return config[0];
}

void	handle_ctrlc(int signum) {
	(void)signum;
	std::cout << "\n🛑 Server shutting down gracefully..." << std::endl;
	runner = false;
}

void Server::mainLoop() {
	signal(SIGINT, handle_ctrlc);
	while (runner) {
		setupFdSets();
		int activity = waitForActivity();
		if (activity < 0)
			break;

		handleNewConnections();
		handleClientRequests();
		handleClientResponses();
		handleCgiJobs();
	}
}

void Server::setupFdSets() {
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);

	max_fd = 0;

	// Aggiungi tutti i server socket al set di lettura
	for (size_t i = 0; i < serverSockets.size(); ++i) {
		int server_fd = serverSockets[i].getFd();
		FD_SET(server_fd, &read_fds);
		if (server_fd > max_fd)
			max_fd = server_fd;
	}

	for (int i = 0; i < MAX_CLIENTS; i++) {
		int client_fd = clients[i].getFd();
		if (client_fd > -1) {
			FD_SET(client_fd, &read_fds);

			if (!client_output_buffer[client_fd].empty())
				FD_SET(client_fd, &write_fds);

			if (client_fd > max_fd)
				max_fd = client_fd;
		}
	}
	for (size_t i = 0; i < cgi_tasks.size(); ++i) {
		int pfd = cgi_tasks[i].pipe_fd;
		FD_SET(pfd, &read_fds);
		if (pfd > max_fd)
			max_fd = pfd;
	}
}

int Server::waitForActivity() {
	struct timeval timeout = {0, 0};
	int activity = select(max_fd + 1, &read_fds, &write_fds, NULL, &timeout);

	if (activity < 0) {
		perror("select");
		return -1;
	}
	return activity;
}

void Server::handleNewConnections() {
	// Controlla tutti i server socket per nuove connessioni
	for (size_t i = 0; i < serverSockets.size(); ++i) {
		int server_fd = serverSockets[i].getFd();
		if (FD_ISSET(server_fd, &read_fds)) {
			try {
				int new_client = serverSockets[i].accept();
				addClient(new_client, i);  // Passa l'indice del server
			} catch (const SocketException &e) {
				std::cerr << "Accept failed on server " << i << ": " << e.what() << std::endl;
			}
		}
	}
}

void Server::handleClientRequests() {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		int client_fd = clients[i].getFd();
		if (client_fd != -1 && FD_ISSET(client_fd, &read_fds)) {
			char buffer[BUF_SIZE];
			ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);

			if (bytes_read <= 0) {
				close(client_fd);
				clients[i].setFd(-1);
				client_input_buffer.erase(client_fd);
				clientToConfigIndex.erase(client_fd);
			} else {
				// FIX: Non aggiungere \0 per preservare i dati binari
				client_input_buffer[client_fd].append(buffer, bytes_read);

				// Cerca il separatore header-body
				size_t headerEnd = client_input_buffer[client_fd].find("\r\n\r\n");
				if (headerEnd != std::string::npos) {
					// Cerca Content-Length
					std::string headers = client_input_buffer[client_fd].substr(0, headerEnd);
					size_t contentLengthPos = headers.find("Content-Length:");
					size_t totalLength = headerEnd + 4;
					if (contentLengthPos != std::string::npos) {
						size_t valueStart = contentLengthPos + 15;
						size_t valueEnd = headers.find("\r\n", valueStart);
						std::string lenStr = headers.substr(valueStart, valueEnd - valueStart);
						// FIX: Rimuovi spazi iniziali dal valore Content-Length
						size_t firstNonSpace = lenStr.find_first_not_of(" \t");
						if (firstNonSpace != std::string::npos)
							lenStr = lenStr.substr(firstNonSpace);
						int contentLength = atoi(lenStr.c_str());
						totalLength += contentLength;
					}
					// Se abbiamo tutta la richiesta, processa
					if (client_input_buffer[client_fd].size() >= totalLength) {
						handleRequest(client_fd, client_input_buffer[client_fd].substr(0, totalLength));
						client_input_buffer[client_fd].erase(0, totalLength);
					}
				}
			}
		}
	}
}

void Server::handleClientResponses() {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		int client_fd = clients[i].getFd();
		if (client_fd != -1 && FD_ISSET(client_fd, &write_fds)) {
			std::string &response = client_output_buffer[client_fd];
			if (!response.empty()) {
				ssize_t sent = send(client_fd, response.c_str(), response.size(), 0);
				if (sent > 0) {
					response.erase(0, sent);
					if (response.empty()) {
						// Tutto inviato, possiamo pulire il buffer
						client_output_buffer.erase(client_fd);

						// Se la connessione deve chiudersi, chiudila qui
						close(client_fd);
						clients[i].setFd(-1);
						clientToConfigIndex.erase(client_fd);
					}
				} else if (sent == -1) {
					// Errore serio, chiudi connessione
					close(client_fd);
					clients[i].setFd(-1);
					client_output_buffer.erase(client_fd);
					clientToConfigIndex.erase(client_fd);
				}
			}
		}
	}
}

// New helper to close client cleanly
void Server::closeClient(int client_fd) {
    close(client_fd);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].getFd() == client_fd) {
            clients[i].setFd(-1);
            break;
        }
    }
    client_input_buffer.erase(client_fd);
    client_output_buffer.erase(client_fd);
    clientToConfigIndex.erase(client_fd);  // Pulisci anche la mappatura
}

// Updated handleCgiJobs (formerly handleFinishedCgi):
void Server::handleCgiJobs() {
    time_t now = time(NULL);

    for (std::vector<CgiTask>::iterator it = cgi_tasks.begin(); it != cgi_tasks.end(); /* in-loop */) {
        bool done = false;
        
        // Case 1: Timeout handling (check first!)
        if (now - it->start_time > CGI_TIMEOUT) {
            std::cout << RED << "Handle CGI JOB Case 1: Timeout handling" << RESET << std::endl;
            std::cout << YELLOW << "TIMEOUT OCCUR:\ttimeout: " << RESET << CGI_TIMEOUT<< std::endl;
            kill(it->pid, SIGKILL);
            close(it->pipe_fd);
            HttpResponse resp;
            resp.setErrorResponse(504, "CGI process timed out");
            std::string s = resp.toString();
            send(it->client_fd, s.c_str(), s.size(), 0);
            closeClient(it->client_fd);
            done = true;
        }
        // Case 2: Pipe is ready for reading
        else if (FD_ISSET(it->pipe_fd, &read_fds)) {
            char buf[4096];
            ssize_t n = read(it->pipe_fd, buf, sizeof(buf));
            if (n > 0) {
                it->buffer.append(buf, n);
            } else if (n == 0) {
                // EOF - CGI finished
                close(it->pipe_fd);
                int status = 0;
                waitpid(it->pid, &status, 0); // Reap child
                // Send response to client
                HttpResponse resp(200);
                resp.setBody(it->buffer);
                resp.addHeader("Content-Type", "text/html");
                std::string s = resp.toString();
                send(it->client_fd, s.c_str(), s.size(), 0);
                // Clean up client
                //closeClient(it->client_fd);
                done = true;
            } else if (n < 0) {
                perror("read");
                done = true;
                // TODO: Need to clean up resources (close pipe, kill process)
                close(it->pipe_fd);
                kill(it->pid, SIGKILL);
                waitpid(it->pid, NULL, 0);
                done = true;
            }
        }

        // Remove finished task
        if (done)
            it = cgi_tasks.erase(it);
        else
			++it;
    }
}


void Server::handleRequest(int client_fd, const std::string &data) {
    try {
        const ServerConfig& serverConfig = getConfigForClient(client_fd);
        HttpRequest req(data, serverConfig);
        const std::string& method = req.getMethod();
        HttpResponse response;

        // Prima controlliamo se il metodo è supportato dal server
        if (method != "GET" && method != "POST" && method != "DELETE") {
            response.setErrorResponse(405);
            std::string responseStr = response.toString();
            send(client_fd, responseStr.c_str(), responseStr.size(), 0);
            return;
        }

		if (!req.hasHeader("Cookie")) {
		std::string sessionId = generateRandomSessionId();  // You create this helper
		response.addSetCookie("session_id", sessionId, "/", 36, true);
		// Optionally: store session in map[session_id] if needed
		}

        if (method == "GET")
            response = handleGet(req, client_fd);
        else if (method == "POST")
            response = handlePost(req);
        else if (method == "DELETE")
            response = handleDelete(req);

        // Send response ONLY if status code is non-zero -->
        if (response.getStatusCode() != 0) {
            std::string responseStr = response.toString();
            send(client_fd, responseStr.c_str(), responseStr.size(), 0);
		}

        // Chiudi solo se l'header dice Connection: close
        if (req.hasHeader("Connection") && req.getHeader("Connection") == "close") {
            close(client_fd);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].getFd() == client_fd) {
                    clients[i].setFd(-1);
                    break;
                }
            }
            client_input_buffer.erase(client_fd);
            clientToConfigIndex.erase(client_fd);
        }
        // Altrimenti lascia la connessione aperta per altre richieste
    } catch (const UnsupportedMethodException& ex) {
        HttpResponse errorResponse;
        errorResponse.setErrorResponse(405, ex.what());
        std::string errorStr = errorResponse.toString();
        send(client_fd, errorStr.c_str(), errorStr.size(), 0);
        close(client_fd);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].getFd() == client_fd) {
                clients[i].setFd(-1);
                break;
            }
        }
        client_input_buffer.erase(client_fd);
        clientToConfigIndex.erase(client_fd);
    } catch (const InvalidHttpRequestException& ex) {
        HttpResponse errorResponse;
        errorResponse.setErrorResponse(400, ex.what());
        std::string errorStr = errorResponse.toString();
        send(client_fd, errorStr.c_str(), errorStr.size(), 0);
        close(client_fd);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].getFd() == client_fd) {
                clients[i].setFd(-1);
                break;
            }
        }
        client_input_buffer.erase(client_fd);
        clientToConfigIndex.erase(client_fd);
    } catch (const ServerErrorException& ex) {
        HttpResponse errorResponse;
        errorResponse.setErrorResponse(500, ex.what());
        std::string errorStr = errorResponse.toString();
        send(client_fd, errorStr.c_str(), errorStr.size(), 0);
        closeClient(client_fd);
    } catch (const HttpException& ex) {
        HttpResponse errorResponse;
        errorResponse.setErrorResponse(500, ex.what());
        std::string errorStr = errorResponse.toString();
        send(client_fd, errorStr.c_str(), errorStr.size(), 0);
        closeClient(client_fd);
    }
}

//-----------------------------------------------------------------------------
// Return true if 'path' ends in one of the configured CGI extensions.
static bool isCgiRequest(const LocationConfig& loc, const std::string& path) {
	const std::vector<std::string>& exts = loc.getCgiExtensions();

	for (size_t i = 0; i < exts.size(); ++i) {
		const std::string& ext = exts[i];
		if (path.size() >= ext.size() && path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
			std::cout << GREEN << "CGI: has been requested!" << RESET << std::endl;
			return true;
		}
	}
	return false;
}

static std::string urlDecode(const std::string& str) {
	std::string result;
	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] == '%' && i + 2 < str.length()) {
			std::istringstream iss(str.substr(i + 1, 2));
			int hex;
			if (iss >> std::hex >> hex)
				result += static_cast<char>(hex);
			i += 2;
		} else if (str[i] == '+') {
			result += ' ';
		} else {
			result += str[i];
		}
	}
	return result;
}

HttpResponse Server::handleGet(const HttpRequest& req, int client_fd) {
	const LocationConfig& loc = req.getLocation();

	if (loc.hasRedirect()) {
        return handleRedirect(loc.getRedirectCode(), loc.getRedirectUrl());
    }
    
    std::string root = loc.getRoot();
    std::string target = urlDecode(req.getTarget());
    std::string file = urlDecode(req.getFile());
    std::string basePath = "." + root + target;

    if (!basePath.empty() && basePath[basePath.size() - 1] != '/')
        basePath += '/';

    std::string fullPath = basePath + file;
    
    if (isCgiRequest(loc, fullPath)) {
        std::cout << RED << "CGI: has been requested!" << RESET << std::endl;
        std::cout << YELLOW << "CGI: path: " << RESET << fullPath << std::endl;
        Cgi::CgiSpawn job = Cgi().spawn(fullPath);
        CgiTask task;
        task.client_fd = client_fd;
        task.pid = job.pid;
        task.pipe_fd = job.read_fd;
        task.start_time = time(NULL);
        task.buffer = "";
        cgi_tasks.push_back(task);
        return HttpResponse(0);  
    }

    struct stat st;

	std::string uploadDir = "." + loc.getUploadDir();
	if (stat(uploadDir.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
		HttpResponse res;
		res.setErrorResponse(500, "Upload directory does not exist: " + uploadDir);
		return res;
	}

    // Caso 1: il file è specificato
    if (!file.empty()) {
        if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
            return serveFile(fullPath);
        } else {
            HttpResponse a;
            a.setErrorResponse(404);
            return a;
        }
    }
    
    // Caso 2: directory request
    if (stat(basePath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
        // Cerca un file indice nella directory
        std::vector<std::string> index = loc.getIndexPages();
        for (size_t i = 0; i < index.size(); ++i) {
            std::string indexFile = basePath + index[i];
            struct stat stIndex;
            if (stat(indexFile.c_str(), &stIndex) == 0 && S_ISREG(stIndex.st_mode)) {
                return serveFile(indexFile);
            }
        }
        
        // Se non c'è un file indice, controlla se autoindex è abilitato
		std::cout << "\n\n\n\n" << loc.getAutoIndex() << "---" << std::endl;
        if (loc.getAutoIndex()) {
            return autoindex(basePath, req);
        } else {
			std::cout << "\n\n\n403\n";
            HttpResponse a;
            a.setErrorResponse(403);
            return a;
        }
    }
	std::cout << "\n\n\n404\n";

    // Caso 3: né file né directory trovati
    HttpResponse a;
    a.setErrorResponse(404);
    return a;
}

HttpResponse Server::handleRedirect(int code, const std::string& target) {
    HttpResponse response(code);
    response.addHeader("Location", target);
    response.addHeader("Content-Type", "text/html");
    response.setBody(generateRedirectHTML(code, target));
    return response;
}

std::string Server::generateRedirectHTML(int code, const std::string& target) {
    std::ostringstream html;
    std::string title, message;
    
    switch (code) {
        case 301: title = "Moved Permanently"; message = "permanently moved"; break;
        case 302: title = "Found"; message = "temporarily moved"; break;
        case 307: title = "Temporary Redirect"; message = "temporarily redirected"; break;
        case 308: title = "Permanent Redirect"; message = "permanently redirected"; break;
        default: title = "Redirect"; message = "moved"; break;
    }
    
    html << "<!DOCTYPE html>\n<html>\n<head>\n"
         << "<title>" << code << " " << title << "</title>\n"
         << "<meta http-equiv=\"refresh\" content=\"3;url=" << target << "\">\n"
         << "</head>\n<body>\n"
         << "<h1>" << title << "</h1>\n"
         << "<p>The document has been " << message << " to "
         << "<a href=\"" << target << "\">" << target << "</a>.</p>\n"
         << "<p>You will be redirected automatically in 3 seconds.</p>\n"
         << "</body>\n</html>\n";
    
    return html.str();
}

HttpResponse Server::handlePost(const HttpRequest& req) {
	const LocationConfig& loc = req.getLocation();
	std::string uploadDir = "." + loc.getUploadDir();
	
	HttpResponse res;
	struct stat st;
	if (stat(uploadDir.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
		res.setErrorResponse(500, "Upload directory does not exist: " + uploadDir);
		return res;
	}
	
	std::string contentType = req.getHeader("Content-Type");
	
	try {
		if (req.getTarget() == "/file")
			return handleMultipartPost(req, uploadDir);
		else if (req.getTarget() == "/json")
			return handleJsonPost(req, uploadDir);
		else if (req.getTarget() == "/raw")
			return handleRawPost(req, uploadDir);
		else {
			res.setErrorResponse(415);
			return res;
		}
	} catch (const InvalidHttpRequestException& e) {
		res.setErrorResponse(400, "Invalid request: " + std::string(e.what()));
		return res;
	} catch (const ServerErrorException& e) {
		res.setErrorResponse(500, "Server error: " + std::string(e.what()));
		return res;
	} catch (const HttpException& e) {
		res.setErrorResponse(400, "Error processing POST request: " + std::string(e.what()));
		return res;
	}
}

HttpResponse Server::handleMultipartPost(const HttpRequest& req, const std::string& uploadDir) {
	std::string filename = req.extractMultipartFilename();
	if (filename.empty())
		filename = "uploaded_file.bin";

	if (isScriptingFile(filename)) {
        HttpResponse res;
        res.setErrorResponse(415, "Unsupported media type. Scripting files are not allowed.");
        return res;
    }
	std::string fileContent = req.extractMultipartFileContent();
	if (fileContent.empty()) {
		HttpResponse res;
		res.setErrorResponse(400, "No file uploaded");
		return res;
	}
	std::string fullPath = uploadDir + "/" + filename;
	std::ofstream file(fullPath.c_str(), std::ios::binary);
	if (!file) {
		HttpResponse res;
		res.setErrorResponse(500, "Cannot save file: " + filename);
		return res;
	}
	file.write(fileContent.c_str(), fileContent.size());
	file.close();
	HttpResponse res(201);
	res.setBody("File saved successfully as: " + filename);
	res.addHeader("Content-Type", "text/plain");
	return res;
}

HttpResponse Server::handleJsonPost(const HttpRequest& req, const std::string& uploadDir) {
	std::string content;
	std::string filename = "data.json";
	std::string contentType = req.getHeader("Content-Type");

	// 1. Prova a leggere il nome file dall'header custom X-Filename
	if (req.hasHeader("X-Filename")) {
		std::string fn = req.getHeader("X-Filename");
		// Sanifica: niente path traversal o slash
		if (!fn.empty() && fn.find("..") == std::string::npos && fn.find('/') == std::string::npos)
			filename = fn;
		if (filename.find(".json") == std::string::npos)
			filename += ".json";
	}

	// 2. Se multipart, estrai anche da campo form
	if (contentType.find("application/json") != std::string::npos) {
		content = req.getBody();
	} else if (contentType.find("multipart/form-data") != std::string::npos) {
		content = req.extractMultipartField("json");
		std::string fn = req.extractMultipartField("filename");
		if (!fn.empty() && fn.find("..") == std::string::npos && fn.find('/') == std::string::npos)
			filename = fn;
		if (filename.find(".json") == std::string::npos)
			filename += ".json";
	} else {
		HttpResponse res;
		res.setErrorResponse(415, "Unsupported Content-Type for JSON");
		return res;
	}

	std::string fullPath = uploadDir + "/" + filename;
	std::ofstream file(fullPath.c_str());
	if (!file) {
		HttpResponse res;
		res.setErrorResponse(500, "Failed to create JSON file");
		return res;
	}
	file << content;
	file.close();
	HttpResponse res(201);
	res.setBody("JSON saved successfully as: " + filename);
	return res;
}

HttpResponse Server::handleRawPost(const HttpRequest& req, const std::string& uploadDir) {
	std::string content;
	std::string filename = "data.txt";
	std::string contentType = req.getHeader("Content-Type");

	// 1. Prova a leggere il nome file dall'header custom X-Filename
	if (req.hasHeader("X-Filename")) {
		std::string fn = req.getHeader("X-Filename");
		// Sanifica: niente path traversal o slash
		if (!fn.empty() && fn.find("..") == std::string::npos && fn.find('/') == std::string::npos)
			filename = fn;
		if (filename.find('.') == std::string::npos)
			filename += ".txt";
	}

	// 2. Se multipart, estrai anche da campo form
	if (contentType.find("text/plain") != std::string::npos) {
		content = req.getBody();
	} else if (contentType.find("multipart/form-data") != std::string::npos) {
		content = req.extractMultipartField("raw");
		std::string fn = req.extractMultipartField("filename");
		if (!fn.empty() && fn.find("..") == std::string::npos && fn.find('/') == std::string::npos)
			filename = fn;
		if (filename.find('.') == std::string::npos)
			filename += ".txt";
	} else {
		HttpResponse res;
		res.setErrorResponse(415, "Unsupported Content-Type for RAW");
		return res;
	}

	std::string fullPath = uploadDir + "/" + filename;
	std::ofstream file(fullPath.c_str());
	if (!file) {
		HttpResponse res;
		res.setErrorResponse(500, "Failed to create raw file");
		return res;
	}
	file << content;
	file.close();
	HttpResponse res(201);
	res.setBody("Raw content saved successfully as: " + filename);
	return res;
}

HttpResponse Server::handleDelete(const HttpRequest& req) {
	const LocationConfig& loc = req.getLocation();
	std::string root = loc.getRoot();           // es: /var/www
	std::string path = req.getTarget();         // es: /upload/
	std::string file = req.getFile();           // es: image.png

	std::string fullPath = "." + root + path + file;

	if (remove(fullPath.c_str()) != 0) {
		HttpResponse res;
		res.setErrorResponse(404, "File Not Found or Cannot Delete");
		return res;
	}

	HttpResponse res(200);
	res.setBody("File Deleted");
	return res;
}

HttpResponse Server::autoindex(const std::string& dirPath, const HttpRequest& req) {
    // Try to open the directory
    DIR* dp = opendir(dirPath.c_str());
    if (!dp) {
        HttpResponse res;
        res.setErrorResponse(500, "Cannot open directory for listing");
        return res;
    }

    // Controlla se il client vuole JSON (per API JavaScript)
    bool wantsJson = false;
    if (req.hasHeader("Accept")) {
        std::string accept = req.getHeader("Accept");
        if (accept.find("application/json") != std::string::npos) {
            wantsJson = true;
        }
    }

    if (wantsJson) {
        // Restituisci JSON per le API JavaScript
        std::ostringstream json;
        json << "[";
        
        struct dirent* entry;
        bool first = true;
        while ((entry = readdir(dp)) != 0) {
            const char* name = entry->d_name;
            if (std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0)
                continue;
            
            if (!first) json << ",";
            json << "\"" << name << "\"";
            first = false;
        }
        
        json << "]";
        closedir(dp);
        
        HttpResponse res(200);
        res.setBody(json.str());
        res.addHeader("Content-Type", "application/json");
        return res;
    } else {
        // Restituisci HTML normale per il browser
        std::ostringstream html;
        std::string target = req.getTarget();
        
        // ensure target ends with '/'
        if (target.empty() || target[target.size() - 1] != '/')
            target += '/';

        html << "<!DOCTYPE html>\n"
             << "<html>\n<head>\n"
             << "  <meta charset=\"utf-8\">\n"
             << "  <title>Index of " << target << "</title>\n"
             << "</head>\n<body>\n"
             << "  <h1>Index of " << target << "</h1>\n"
             << "  <ul>\n";

        struct dirent* entry;
        while ((entry = readdir(dp)) != 0) {
            const char* name = entry->d_name;
            if (std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0)
                continue;

            std::string link = target + name;

            html << "    <li><a href=\""
                 << link << "\">"
                 << name << "</a></li>\n";
        }

        html << "  </ul>\n</body>\n</html>\n";
        closedir(dp);

        HttpResponse res(200);
        res.setBody(html.str());
        res.addHeader("Content-Type", "text/html");
        return res;
    }
}

HttpResponse Server::serveFile(const std::string& filePath) {
    std::ifstream file(filePath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        HttpResponse res;
        res.setErrorResponse(404);
        return res;
    }

    std::ostringstream fileContent;
    fileContent << file.rdbuf();
    std::string body = fileContent.str();
    
    HttpResponse res(200);
    res.setBody(body);
    
    std::string mimeType = getMimeType(filePath);
    res.addHeader("Content-Type", mimeType);
    
    std::ostringstream oss;
    oss << body.size();
    res.addHeader("Content-Length", oss.str());
    
    // Per le immagini, aggiungi header specifici
    if (mimeType.find("image/") == 0) {
        res.addHeader("Cache-Control", "public, max-age=86400"); // Cache per 1 giorno
        res.addHeader("Accept-Ranges", "bytes");
    }
    
    return res;
}

std::string Server::getMimeType(const std::string& filePath) {
    size_t pos = filePath.rfind('.');
    if (pos == std::string::npos) return "application/octet-stream";
    
    std::string ext = filePath.substr(pos + 1);
    
    // Converti in lowercase per evitare problemi con maiuscole/minuscole
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    // Mappa delle estensioni a MIME types
    if (ext == "html" || ext == "htm") return "text/html";
    if (ext == "css") return "text/css";
    if (ext == "js") return "application/javascript";
    if (ext == "png") return "image/png";
    if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
    if (ext == "gif") return "image/gif";
    if (ext == "svg") return "image/svg+xml";
    if (ext == "bmp") return "image/bmp";
    if (ext == "webp") return "image/webp";
    if (ext == "ico") return "image/x-icon";
    if (ext == "txt") return "text/plain";
    if (ext == "json") return "application/json";
    if (ext == "xml") return "application/xml";
    if (ext == "pdf") return "application/pdf";
    if (ext == "zip") return "application/zip";
    if (ext == "mp3") return "audio/mpeg";
    if (ext == "mp4") return "video/mp4";
    
    // Tipo di default per file sconosciuti
    return "application/octet-stream";
}

bool Server::isScriptingFile(const std::string& filename) {
    std::string lowerFilename = filename;
    std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
    
    // Lista dei file di scripting pericolosi
    std::vector<std::string> scriptingExtensions;
    scriptingExtensions.push_back(".php");
    scriptingExtensions.push_back(".py");
    scriptingExtensions.push_back(".pl");
    scriptingExtensions.push_back(".rb");
    scriptingExtensions.push_back(".sh");
    scriptingExtensions.push_back(".bat");
    scriptingExtensions.push_back(".cmd");
    scriptingExtensions.push_back(".exe");
    scriptingExtensions.push_back(".com");
    scriptingExtensions.push_back(".scr");
    scriptingExtensions.push_back(".vbs");
    scriptingExtensions.push_back(".js");
    scriptingExtensions.push_back(".jar");
    
    for (size_t i = 0; i < scriptingExtensions.size(); ++i) {
        if (lowerFilename.find(scriptingExtensions[i]) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

void Server::setNonBlocking(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw SocketException("fcntl get failed");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw SocketException("fcntl set failed");
}

void Server::addClient(int new_client, size_t server_index) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].getFd() == -1) {
			clients[i].setFd(new_client);
			setNonBlocking(new_client);
			// Mappa il client al server che l'ha accettato
			clientToConfigIndex[new_client] = server_index;
			std::cout << "✅ Client " << new_client << " connected to server " << server_index 
					  << " (port " << config[server_index].getPort() << ")" << std::endl;
			return;
		}
	}
	std::cerr << "[-] Max clients reached. Rejecting fd: " << new_client << std::endl;
	close(new_client);
}

std::string Server::generateRandomSessionId() const {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string id;
    for (int i = 0; i < 16; ++i)
        id += alphanum[rand() % (sizeof(alphanum) - 1)];
    return id;
}

const std::vector<ServerConfig> Server::getConfig() const {
	return config;
}

const std::vector<Socket> Server::getClients() const {
	return clients;
}

const std::vector<Socket>& Server::getServerSockets() const {
	return serverSockets;
}

std::ostream& operator<<(std::ostream& os, const Server& ws) {
	os << "Server {\n  config: [\n";
	for (size_t i = 0; i < ws.getConfig().size(); ++i) {
		os << "    " << ws.getConfig()[i] << ",\n";
	}
	os << "  ],\n"
	   << "  serverSockets: " << ws.getServerSockets().size() << " socket(s)\n"
	   << "}";
	return os;
}
