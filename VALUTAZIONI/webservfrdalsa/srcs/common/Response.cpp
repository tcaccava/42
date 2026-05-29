#include "Response.hpp"
#include "Config.hpp"
#include <sstream>
#include <poll.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <signal.h>
#include <ctime>
#include <cstdlib>
#include <cerrno>
#include <sys/stat.h>

// Costruttore di default della risposta HTTP
Response::Response(const ServerConfig* config)
	: _statusCode(200), _statusMessage("OK"), _chunkedEncoding(false), _multipart(false), _config(config) {}

// Imposta il codice di stato e la relativa descrizione
void Response::setStatus(int code, const std::string& reason) {
	_statusCode = code;
	_statusMessage = reason;
}

// Aggiunge o sostituisce un header HTTP
void Response::setHeader(const std::string& key, const std::string& value) {
	_headers[key] = value;
}

// Imposta il corpo della risposta
void Response::setBody(const std::string& body) {
	_body = body;
}

// Abilita la codifica chunked nella risposta
void Response::enableChunked(bool enabled) {
	_chunkedEncoding = enabled;
	if (enabled)
		_headers["Transfer-Encoding"] = "chunked";
}

// Indica che la risposta conterrà parti multipart
void Response::enableMultipart(bool enabled) {
	_multipart = enabled;
	if (enabled)
		_headers["Content-Type"] = "multipart/form-data; boundary=--boundary42";
}

// Genera una pagina di errore HTML
void Response::generateErrorPage(int code, const std::string& file) {
	switch (code) {
		case 404: setStatus(404, "Not Found"); break;
		case 405: setStatus(405, "Method Not Allowed"); break;
		case 413: setStatus(413, "Payload Too Large"); break;
		default: setStatus(500, "Internal Server Error"); break;
	}
	std::string path = file;
	if (path.empty() && _config) {
		std::map<int, std::string>::const_iterator it = _config->error_pages.find(code);
		if (it != _config->error_pages.end()) {
			path = _config->root;
			if (!path.empty() && path[path.size() - 1] != '/')
				path += "/";
			std::string ep = it->second;
			if (!ep.empty() && ep[0] == '/')
				ep.erase(0, 1);
			path += ep;
		}
	}

	if (!path.empty()) {
		int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
		if (fd != -1) {
			struct pollfd pfd;
			pfd.fd = fd;
			pfd.events = POLLIN;
			std::ostringstream buffer;
			char tmp[4096];
			while (poll(&pfd, 1, -1) > 0 && (pfd.revents & POLLIN)) {
				ssize_t r = read(fd, tmp, sizeof(tmp));
				if (r > 0)
					buffer.write(tmp, r);
				else if (r == 0)
					break;
				else {
					buffer.str("");
					break;
				}
			}
			close(fd);
			if (!buffer.str().empty()) {
				size_t dot = path.find_last_of('.');
				std::string ext = (dot != std::string::npos) ? path.substr(dot + 1) : "";
				setHeader("Content-Type", getMimeType(ext));
				setBody(buffer.str());
				return;
			}
		}
	}

	std::ostringstream oss;
	oss << "<html><body><h1>" << _statusCode << " " << _statusMessage << "</h1></body></html>";
	setHeader("Content-Type", "text/html");
	setBody(oss.str());
}

// Invia il contenuto di un file come risposta
void Response::serveFile(const std::string& path) {
	int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
	if (fd == -1) {
		generateErrorPage(404);
		return;
	}
	PRINT_DEBUG("Lettura file " << path);

	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	std::ostringstream buffer;
	char tmp[4096];
	while (poll(&pfd, 1, -1) > 0 && (pfd.revents & POLLIN)) {
		ssize_t r = read(fd, tmp, sizeof(tmp));
		if (r > 0)
			buffer.write(tmp, r);
		else if (r == 0)
			break;
		else {
			close(fd);
			generateErrorPage(500);
			return;
		}
	}
	close(fd);

	size_t dot = path.find_last_of(".");
	std::string ext = (dot != std::string::npos) ? path.substr(dot + 1) : "";
	setHeader("Content-Type", getMimeType(ext));

	std::ostringstream len;
	len << buffer.str().size();
	setHeader("Content-Length", len.str());

	setBody(buffer.str());
}

// Genera una pagina HTML con l'elenco dei file in una directory
void Response::serveDirectoryListing(const std::string& dirPath) {
	DIR* dir = opendir(dirPath.c_str());
	if (!dir) {
		generateErrorPage(404);
		return;
	}
	PRINT_DEBUG("Autoindex di " << dirPath);

	std::ostringstream html;
	html << "<html><body><h1>Index of " << dirPath << "</h1><hr><pre>";

	struct dirent* entry;
	while ((entry = readdir(dir))) {
		std::string name(entry->d_name);
		if (name == ".") continue;

		html << "<a href=\"" << escapeHtml(name) << "\">" << escapeHtml(name) << "</a>\n";
	}
	closedir(dir);
	html << "</pre><hr></body></html>";

	setStatus(200, "OK");
	setHeader("Content-Type", "text/html");
	setBody(html.str());
}

// Gestisce le richieste a directory, effettuando redirect o autoindex
bool Response::handleDirectoryRequest(const Request& req, const LocationConfig* loc,
									  std::string& fullPath) {
	struct stat st;
	if (::stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
		PRINT_DEBUG("Richiesta directory " << fullPath);

		if (!req.getPath().empty() && req.getPath()[req.getPath().size() - 1] != '/') {
			std::string newLocation = req.getPath() + "/";
			redirect(newLocation, 301);
			return true;
		}

		std::string indexFile = (loc && !loc->index.empty()) ? loc->index : "index.html";
		std::string indexPath = fullPath;
		if (!indexPath.empty() && indexPath[indexPath.size() - 1] != '/')
			indexPath += "/";
		indexPath += indexFile;

		if (::stat(indexPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
			fullPath = indexPath;
		} else if (loc && loc->autoindex) {
			serveDirectoryListing(fullPath);
			return true;
		} else if (!req.getPath().empty() && req.getPath()[req.getPath().size() - 1] != '/') {
			std::string newLocation = req.getPath() + "/";
			redirect(newLocation, 301);
			return true;
		} else {
			generateErrorPage(404);
			return true;
		}
	}
	return false;
}

// Invia un redirect al client verso l'URL indicato
void Response::redirect(const std::string& url, int code) {
	setStatus(code, "Moved");
	setHeader("Location", url);
	setBody("");
}

// Gestisce l'upload di file in formato multipart/form-data
void Response::handleMultipartUpload(const std::string& requestBody, const std::string& uploadDir) {
	size_t pos = requestBody.find("\r\n");
	if (pos == std::string::npos) {
		generateErrorPage(400);
		return;
	}
	std::string boundary = requestBody.substr(0, pos);
	bool saved = false;
	size_t cur = 0;
	while ((pos = requestBody.find(boundary, cur)) != std::string::npos) {
		cur = pos + boundary.size();
		if (requestBody.compare(cur, 2, "--") == 0)
			break;
		if (requestBody.compare(cur, 2, "\r\n") == 0)
			cur += 2;

		size_t headerEnd = requestBody.find("\r\n\r\n", cur);
		if (headerEnd == std::string::npos)
			break;
		std::string headers = requestBody.substr(cur, headerEnd - cur);
		cur = headerEnd + 4;

		size_t next = requestBody.find(boundary, cur);
		if (next == std::string::npos)
			break;
		size_t contentLen = next - cur;
		if (contentLen >= 2 && requestBody[next - 2] == '\r' && requestBody[next - 1] == '\n')
			contentLen -= 2;
		std::string content = requestBody.substr(cur, contentLen);
		cur = next;

		size_t fnPos = headers.find("filename=\"");
		if (fnPos != std::string::npos) {
			size_t fnStart = fnPos + 10;
			size_t fnEnd = headers.find('"', fnStart);
			std::string filename = headers.substr(fnStart, fnEnd - fnStart);
			if (!filename.empty()) {
				std::string path = uploadDir;
				if (!path.empty() && path[path.size() - 1] != '/')
					path += "/";
				path += filename;
				int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK, 0644);
				if (fd == -1) {
					generateErrorPage(500);
					return;
				}
				struct pollfd pfd;
				pfd.fd = fd;
				pfd.events = POLLOUT;
				size_t written = 0;
				while (written < content.size()) {
					if (poll(&pfd, 1, -1) <= 0 || !(pfd.revents & POLLOUT)) {
						close(fd);
						generateErrorPage(500);
						return;
					}
					ssize_t w = write(fd, content.c_str() + written, content.size() - written);
					if (w > 0)
						written += w;
					else {
						close(fd);
						generateErrorPage(500);
						return;
					}
				}
				close(fd);
				saved = true;
			}
		}
	}

	if (saved) {
		setStatus(201, "Created");
		setHeader("Content-Type", "text/html");
		setBody("<html><body><h1>Upload successful</h1></body></html>");
	} else {
		generateErrorPage(400);
	}
}

// Inizializza processo e pipe per l'esecuzione di uno script CGI
bool Response::_setupCgi(const std::string& interpreter, const std::string& scriptPath,
						 int timeout, int inPipe[2], int outPipe[2],
						 int& timerFd, pid_t& pid) {
	timerFd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (timerFd == -1)
		return false;

	struct itimerspec ts;
	ts.it_value.tv_sec = timeout;
	ts.it_value.tv_nsec = 0;
	ts.it_interval.tv_sec = 0;
	ts.it_interval.tv_nsec = 0;
	if (timerfd_settime(timerFd, 0, &ts, NULL) == -1) {
		close(timerFd);
		return false;
	}

	if (pipe(inPipe) == -1 || pipe(outPipe) == -1) {
		close(timerFd);
		return false;
	}

	pid = fork();
	if (pid < 0) {
		close(timerFd);
		close(inPipe[0]); close(inPipe[1]);
		close(outPipe[0]); close(outPipe[1]);
		return false;
	}
	if (pid == 0) {
		dup2(inPipe[0], STDIN_FILENO);
		dup2(outPipe[1], STDOUT_FILENO);
		close(inPipe[1]); close(outPipe[0]);
		execl(interpreter.c_str(), interpreter.c_str(), scriptPath.c_str(), (char*)NULL);
		std::_Exit(1);
	}

	close(inPipe[0]);
	close(outPipe[1]);

	int flags = fcntl(inPipe[1], F_GETFL, 0);
	if (flags != -1)
		fcntl(inPipe[1], F_SETFL, flags | O_NONBLOCK);

	flags = fcntl(outPipe[0], F_GETFL, 0);
	if (flags != -1)
		fcntl(outPipe[0], F_SETFL, flags | O_NONBLOCK);

	return true;
}

// Ciclo di attesa per la comunicazione con il processo CGI
void Response::_cgiEventLoop(int inPipe[2], int outPipe[2], int timerFd,
							 const std::string& input, std::ostringstream& output,
							 bool& timedOut, bool& writeError) {
	bool writeClosed = input.empty();

	int epfd = epoll_create1(0);
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = outPipe[0];
	epoll_ctl(epfd, EPOLL_CTL_ADD, outPipe[0], &ev);
	ev.data.fd = timerFd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, timerFd, &ev);
	if (!writeClosed) {
		ev.events = EPOLLOUT;
		ev.data.fd = inPipe[1];
		epoll_ctl(epfd, EPOLL_CTL_ADD, inPipe[1], &ev);
	}

	size_t written = 0;
	char buff[1024];
	bool done = false;
	while (!done) {
		struct epoll_event events[3];
		int nfds = epoll_wait(epfd, events, 3, -1);
		if (nfds <= 0)
			continue;
		for (int i = 0; i < nfds; ++i) {
			if (events[i].data.fd == outPipe[0]) {
				ssize_t r = read(outPipe[0], buff, sizeof(buff));
				if (r > 0)
					output.write(buff, r);
				else if (r == 0) {
					close(outPipe[0]);
					done = true;
				}
				else {
					generateErrorPage(500);
					return;
				}
			} else if (events[i].data.fd == timerFd) {
				timedOut = true;
				uint64_t t;
				int ret = read(timerFd, &t, sizeof(t));
				if (ret == -1) {
					generateErrorPage(500);
					return;
				}
				if (ret == 0)
					done = true;
				else
					done = true;
			} else if (events[i].data.fd == inPipe[1]) {
				ssize_t w = write(inPipe[1], input.c_str() + written, input.size() - written);
				if (w > 0) {
					written += w;
					if (written == input.size()) {
						epoll_ctl(epfd, EPOLL_CTL_DEL, inPipe[1], NULL);
						close(inPipe[1]);
						writeClosed = true;
					}
				} else {
					writeError = true;
					epoll_ctl(epfd, EPOLL_CTL_DEL, inPipe[1], NULL);
					close(inPipe[1]);
					writeClosed = true;
					done = true;
				}
			}
		}
	}

	if (!writeClosed)
		close(inPipe[1]);
	close(epfd);
	close(timerFd);
}

// Analizza l'output dello script CGI e popola header e body
void Response::_parseCgiOutput(const std::string& outStr) {
	size_t headerEnd = outStr.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
		headerEnd = outStr.find("\n\n");

	if (headerEnd != std::string::npos) {
		std::string headerBlock = outStr.substr(0, headerEnd);
		std::istringstream hs(headerBlock);
		std::string line;
		while (std::getline(hs, line)) {
			if (!line.empty() && line[line.size()-1] == '\r')
				line.erase(line.size()-1);
			size_t colon = line.find(':');
			if (colon != std::string::npos) {
				std::string key = line.substr(0, colon);
				std::string value = line.substr(colon + 1);
				while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
					value.erase(0,1);
				setHeader(key, value);
			}
		}
		_body = outStr.substr(headerEnd + ((outStr[headerEnd] == '\r') ? 4 : 2));
	} else {
		_body = outStr;
		if (_headers.find("Content-Type") == _headers.end())
			setHeader("Content-Type", "text/html");
	}
	setStatus(200, "OK");
}

// Esegue uno script CGI e costruisce la risposta basandosi sul suo output
void Response::generateCGIResponse(const std::string& interpreter, const std::string& scriptPath, const std::string& input, int timeout) {
	int inPipe[2];
	int outPipe[2];
	int timerFd;
	pid_t pid;

	if (!_setupCgi(interpreter, scriptPath, timeout, inPipe, outPipe, timerFd, pid)) {
		generateErrorPage(500);
		return;
	}

	std::ostringstream output;
	bool timedOut = false;
	bool writeError = false;

	_cgiEventLoop(inPipe, outPipe, timerFd, input, output, timedOut, writeError);

	int status;
	waitpid(pid, &status, 0);
	if (timedOut) {
		kill(pid, SIGKILL);
		generateErrorPage(504);
		return;
	}
	if (writeError) {
		kill(pid, SIGKILL);
		generateErrorPage(500);
		return;
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		generateErrorPage(500);
		return;
	}

	_parseCgiOutput(output.str());
}

// Gestisce una richiesta HTTP DELETE
void Response::handleDelete(const std::string& path) {
	if (::unlink(path.c_str()) == 0) {
		setStatus(200, "OK");
		setHeader("Content-Type", "text/html");
		setBody("<html><body><h1>Resource deleted</h1></body></html>");
	} else if (errno == ENOENT) {
		generateErrorPage(404);
	} else {
		generateErrorPage(500);
	}
}

// Restituisce il MIME type in base all'estensione del file
std::string Response::getMimeType(const std::string& ext) const {
	if (ext == "html") return "text/html";
	if (ext == "css") return "text/css";
	if (ext == "js") return "application/javascript";
	if (ext == "png") return "image/png";
	if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
	if (ext == "txt") return "text/plain";
	return "application/octet-stream";
}

// Converte i caratteri speciali in entità HTML
std::string Response::escapeHtml(const std::string& input) const {
	std::string output;
	for (size_t i = 0; i < input.length(); ++i) {
		switch (input[i]) {
			case '&': output += "&amp;"; break;
			case '<': output += "&lt;"; break;
			case '>': output += "&gt;"; break;
			default: output += input[i]; break;
		}
	}
	return output;
}

// Compone la risposta HTTP completa da inviare al client
std::string Response::build() const {
	std::ostringstream oss;
	oss << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";

	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		oss << it->first << ": " << it->second << "\r\n";

	if (!_chunkedEncoding && !_multipart && !_headers.count("Content-Length"))
		oss << "Content-Length: " << _body.size() << "\r\n";

	oss << "\r\n";
	oss << _body;

	return oss.str();
}
