#include "Connection.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Settings.hpp"
#include <sstream>
#include <ctime>
#include <sys/stat.h>
#include <dirent.h> 

// Costruttore di default
Connection::Connection() : _sock(NULL), _inBuf(), _outBuf(), _config(), _serverPort(0), _lastActivity(time(NULL)), _serverIp(""), _configSelected(false) {}

// Costruttore usato quando il server non è ancora stato selezionato
Connection::Connection(Socket* socket, int serverPort, const std::string& serverIp)
	: _sock(socket), _inBuf(), _outBuf(), _config(), _serverPort(serverPort), _lastActivity(time(NULL)), _serverIp(serverIp), _configSelected(false) {}

// Costruttore che riceve direttamente la configurazione del server
Connection::Connection(Socket* socket, const ServerConfig& config, int serverPort, const std::string& serverIp)
	: _sock(socket), _inBuf(), _outBuf(), _config(config), _serverPort(serverPort), _lastActivity(time(NULL)), _serverIp(serverIp), _configSelected(true) {}

// Distruttore: libera il socket se presente
Connection::~Connection() {
	if (_sock) { delete _sock; _sock = NULL;}
}

// Gestisce l'evento proveniente dal Dispatcher
int Connection::handle(Dispatcher::EventType event) {
	switch (event) {
		case Dispatcher::READ: return _onRead();
		case Dispatcher::WRITE: return _onWrite();
		case Dispatcher::ERROR: return _onClose();
		default: return _onClose();
	}
}

// Lettura dei dati provenienti dal socket
int Connection::_onRead() {
	char buff[BUFFER_SIZE];
	ssize_t bytes_read = ::recv(_sock->getFd(), buff, BUFFER_SIZE, 0);
	if (bytes_read < 0) {
		(void)_onClose(); // ignore error
		return -1;
	}
	if (bytes_read == 0) {
		PRINT_ERR("Client disconnected");
		return _onClose();
	}
	PRINT_DEBUG("Ricevuti " << bytes_read << " byte");
	_inBuf.append(buff, bytes_read);
	_lastActivity = time(NULL);

	Request req;

	// check if it is a partial request
	if (!req.parse(_inBuf) || !req.isComplete()) {
		PRINT_DEBUG("Request not complete, waiting for more data");
		return 0; // wait for remaining data 
	} 
	
	PRINT_DEBUG("Richiesta: " << req.getMethod() << " " << req.getPath());

	if (!_configSelected) {
		std::string host = req.getHeader("Host");
		int port = _serverPort;
		std::string ip = _serverIp;
		const std::vector<ServerConfig>& servers = Settings::instance()->getServers();
		const ServerConfig* match = NULL;
		for (size_t i = 0; i < servers.size(); ++i) {
			ServerAddress addr = servers[i].getAddress();
			if (addr.port == port && addr.ip == ip && servers[i].server_name == host) { match = &servers[i]; break; }
		}
		if (!match) {
			for (size_t i = 0; i < servers.size(); ++i) {
				ServerAddress addr = servers[i].getAddress();
				if (addr.port == port && addr.ip == ip) { match = &servers[i]; break; }
			}
		}
		if (match) _config = *match;
		_configSelected = true;
	}
	return _handleRequest(req);
}

// Gestione completa di una richiesta HTTP
int Connection::_handleRequest(const Request& req) {
	Response res(&_config);
	std::string fullPath = _resolvePath(req);
	PRINT_DEBUG("3 Percorso risolto: " << fullPath);

	if (_checkBodySize(req, res))
		return 0;

	const LocationConfig* bestMatch = _findBestLocation(req.getPath());
	if (bestMatch)
		PRINT_DEBUG("4 Location trovata: " << bestMatch->path);

	if (bestMatch) {
		if (!bestMatch->redirect.empty()) {
			PRINT_DEBUG("Redirect verso " << bestMatch->redirect);
			res.redirect(bestMatch->redirect);
			_sendResponse(res);
			return 0;
		}
		if (!bestMatch->allowed_methods.empty()) {
			bool allowed = false;
			std::istringstream iss(bestMatch->allowed_methods);
			std::string m;
			while (iss >> m) {
				if (m == req.getMethod()) { allowed = true; break; }
			}
			if (!allowed) {
				PRINT_DEBUG("Metodo non consentito");
				res.generateErrorPage(405);
				_sendResponse(res);
				return 0;
			}
		}
	}

	if (_handleUpload(req, res, bestMatch))
		return 0;
	if (res.handleDirectoryRequest(req, bestMatch, fullPath)) {
		PRINT_DEBUG("Gestione directory");
		_sendResponse(res);
		return 0;
	}

	_handleCgi(req, res, bestMatch, fullPath);

	_sendResponse(res);
	return 0;
}

// Calcola il percorso fisico della risorsa richiesta
std::string Connection::_resolvePath(const Request& req) {
	const std::string& path = req.getPath();
	const LocationConfig* bestMatch = _findBestLocation(path);

	std::string fullPath;
	if (bestMatch) {
		fullPath = bestMatch->root;
		if (!fullPath.empty() && fullPath[fullPath.size() - 1] != '/')
			fullPath += "/";
		std::string relative = path.substr(bestMatch->path.length());
		if (!relative.empty() && relative[0] == '/')
			relative.erase(0, 1);
		if (!relative.empty())
			fullPath += relative;
	} else {
		fullPath = _config.root + path;
	}
	return fullPath;
}

// Trova la location più adatta alla path richiesta
const LocationConfig* Connection::_findBestLocation(const std::string& path) const {
	const LocationConfig* bestMatch = NULL;
	size_t maxMatchLen = 0;
	for (size_t i = 0; i < _config.locations.size(); ++i) {
		if (path.find(_config.locations[i].path) == 0 &&
			_config.locations[i].path.length() > maxMatchLen) {
			bestMatch = &(_config.locations[i]);
			maxMatchLen = _config.locations[i].path.length();
		}
	}
	return bestMatch;
}

// Controlla la dimensione del body rispetto al limite configurato
bool Connection::_checkBodySize(const Request& req, Response& res) {
	if (_config.client_max_body_size > 0 &&
		req.getBody().size() > static_cast<size_t>(_config.client_max_body_size)) {
			res.generateErrorPage(413);
			_sendResponse(res);
		return true;
	}
	return false;
}

// Gestisce l'upload di file se configurato
bool Connection::_handleUpload(const Request& req, Response& res, const LocationConfig* loc) {
	if (req.getMethod() == "POST" && loc && !loc->upload_store.empty()) {
		PRINT_DEBUG("Upload in " << loc->upload_store);
		res.handleMultipartUpload(req.getBody(), loc->upload_store);
		_sendResponse(res);
		return true;
	}
	return false;
}


// Gestisce l'esecuzione di script CGI o la consegna di file statici
void Connection::_handleCgi(const Request& req, Response& res,
							const LocationConfig* loc, const std::string& fullPath) {
	struct stat st;
	if (req.getMethod() == "DELETE") {
		res.handleDelete(fullPath);
		return;
	}
	if (loc && !loc->cgi_extension.empty() &&
		fullPath.size() >= loc->cgi_extension.size() &&
		fullPath.compare(fullPath.size() - loc->cgi_extension.size(),
						 loc->cgi_extension.size(), loc->cgi_extension) == 0) {
		PRINT_DEBUG("Esecuzione CGI " << fullPath);
		std::string body = (req.getMethod() == "POST") ? req.getBody() : "";
		res.generateCGIResponse(loc->cgi_path, fullPath, body, _config.cgi_timeout);
	} else {
		PRINT_DEBUG("Consegna file " << fullPath);
		if (::stat(fullPath.c_str(), &st) != 0 || !S_ISREG(st.st_mode))
			res.generateErrorPage(404);
		else
			res.serveFile(fullPath);
	}
}

// Prepara il buffer di uscita e notifica il Dispatcher
void Connection::_sendResponse(Response& res) {
	// reset input
	_outBuf = res.build();

	PRINT_DEBUG("Risposta pronta: " << res.getStatusCode() << " " << res.getStatusMessage() <<
				" (" << _outBuf.size() << " byte)");
	_inBuf.clear();
	int ret = Dispatcher::instance()->mod(_sock->getFd(), Dispatcher::WRITE, this);
	if (ret == -1)
	{
		PRINT_ERR("Error setting socket to write mode");
		(void)_onClose(); // ignore error
	}
	else
		PRINT_DEBUG("Connection set to write mode");
}

// Scrittura dei dati sul socket
int Connection::_onWrite() {
	// send response to client
	ssize_t bytes_sent = ::send(_sock->getFd(), _outBuf.c_str(), _outBuf.size(), 0);
	if (bytes_sent == -1)
	{
		PRINT_ERR("Error sending response");
		(void)_onClose(); // ignore error
		return -1;
	}
	if (bytes_sent == 0)
	{
		PRINT_ERR("Client disconnected");
		return _onClose();
	}
	PRINT_DEBUG("Inviati " << bytes_sent << " byte");
	// for handle partial send, save remaining data and check if we need to send more
	_outBuf.erase(0, bytes_sent);
	_lastActivity = time(NULL);
	if (_outBuf.empty())
	{
		PRINT_DEBUG("Response sent, closing connection");
		return _onClose();
	}
	PRINT_DEBUG("Response not sent completely, waiting for new write event");
	return 0;
}

// Chiusura della connessione e rimozione dal Dispatcher
int Connection::_onClose() {
	int ret = Dispatcher::instance()->del(_sock->getFd());
	PRINT_IF_ERR(ret, "Error removing socket from epoll");
	return ret;
}

int Connection::getFd() const { return _sock != NULL ? _sock->getFd() : -1; }
std::string Connection::getIp() const { return _serverIp; }
int Connection::getPort() const { return _serverPort; }

// Verifica se la connessione è inattiva da troppo tempo
bool Connection::checkTimeout(time_t now) {
	if (now - _lastActivity > CONNECTION_TIMEOUT) {
		_onClose();
		PRINT_DEBUG("Close");
		return true;
	}
	return false;
}
