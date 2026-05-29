#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <sstream>
#include "Request.hpp"
#include "ServerConfig.hpp"

// Classe che rappresenta una risposta HTTP da inviare al client
class Response {
public:
	Response(const ServerConfig* config = NULL); // costruttore di default

	// Imposta codice di stato, header e corpo
	void setStatus(int code, const std::string& reason);
	void setHeader(const std::string& key, const std::string& value);
	void setBody(const std::string& body);

	// Abilita codifica chunked o multipart
	void enableChunked(bool enabled);
	void enableMultipart(bool enabled);

	// Metodi di utilità per costruire la risposta
	void generateErrorPage(int code, const std::string& file = "");
	void serveFile(const std::string& path);
	void redirect(const std::string& url, int code = 301);
	void serveDirectoryListing(const std::string& dirPath);

	// Gestisce path che corrispondono a directory
	bool handleDirectoryRequest(const Request& req, const LocationConfig* loc,
								std::string& fullPath);


	// Esegue uno script CGI
	void generateCGIResponse(const std::string& interpreter, const std::string& scriptPath,
							 const std::string& input, int timeout);

	// Salva sul disco i dati di un upload multipart
	void handleMultipartUpload(const std::string& requestBody, const std::string& uploadDir);

	// Elimina il file indicato da una richiesta DELETE
	void handleDelete(const std::string& path);

	// Restituisce la risposta completa da inviare sulla socket
	std::string build() const;

	int getStatusCode() const { return _statusCode; }
	const std::string& getStatusMessage() const { return _statusMessage; }

private:
	int _statusCode;
	std::string _statusMessage;
	std::map<std::string, std::string> _headers;
	std::string _body;
	bool _chunkedEncoding;
	bool _multipart;
	const ServerConfig* _config;

	// Funzioni di supporto interne per la gestione dei processi CGI
	bool _setupCgi(const std::string& interpreter, const std::string& scriptPath,
				   int timeout, int inPipe[2], int outPipe[2],
				   int& timerFd, pid_t& pid);
	void _cgiEventLoop(int inPipe[2], int outPipe[2], int timerFd,
					   const std::string& input, std::ostringstream& output,
					   bool& timedOut, bool& writeError);
	void _parseCgiOutput(const std::string& outStr);

	std::string getMimeType(const std::string& ext) const;
	std::string escapeHtml(const std::string& input) const;
};

#endif
