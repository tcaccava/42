#include "HttpResponse.hpp"
#include <fstream>
#include <iostream>

HttpResponse::HttpResponse() : statusCode(200), statusMessage("OK") {}

HttpResponse::HttpResponse(int code) : statusCode(code) {
	const std::map<int,std::string> &phrases = getReasonPhrases();
	std::map<int,std::string>::const_iterator it = phrases.find(code);
	if (it != phrases.end())
		statusMessage = it->second;
	else
		statusMessage = "Unknown";
}

HttpResponse::~HttpResponse() {}

HttpResponse::HttpResponse(const HttpResponse &toCopy) {
	*this = toCopy;
}

HttpResponse& HttpResponse::operator=(const HttpResponse &other) {
	if (this != &other) {
		statusCode = other.statusCode;
		statusMessage = other.statusMessage;
		headers = other.headers;
		body = other.body;
	}
	return (*this);
}

void HttpResponse::setBody(const std::string &b) {
    body = b;
}

void HttpResponse::addHeader(const std::string &key, const std::string &value) {
    headers[key] = value;
}

const std::map<int, std::string>& HttpResponse::getReasonPhrases() {
	static std::map<int, std::string> phrases;
	if (phrases.empty()) {
		phrases[0] = "WAIT";
		phrases[200] = "OK";
		phrases[201] = "Created";
		phrases[204] = "No Content";
		phrases[301] = "Moved Permanently";
		phrases[302] = "Found";
		phrases[304] = "Not Modified";
		phrases[400] = "Bad Request";
		phrases[401] = "Unauthorized";
		phrases[403] = "Forbidden";
		phrases[404] = "Not Found";
		phrases[405] = "Method Not Allowed";
		phrases[415] = "Unsupported Media Type";
		phrases[500] = "Internal Server Error";
		phrases[502] = "Bad Gateway";
		phrases[503] = "Service Unavailable";
		phrases[504] = "Gateway Timeout";
	}
	return phrases;
}

std::string HttpResponse::buildStatusLine() const {
	// Format: "HTTP/1.1 <code> <phrase>\r\n"
	std::ostringstream oss;
	oss << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
	return oss.str();
}

std::string HttpResponse::buildHeaders() const {
	std::ostringstream oss;

	// Mandatory headers:
	// 1) Content-Length
	oss << "Content-Length: " << body.size() << "\r\n";
	// 2) Content-Type
	std::map<std::string,std::string>::const_iterator it = headers.find("Content-Type");
	if (it != headers.end())
		oss << "Content-Type: " << it->second << "\r\n";
	else	// default to plain text
		oss << "Content-Type: text/plain\r\n";

	// 3) Connection
	it = headers.find("Connection");
	if (it != headers.end())
		oss << "Connection: " << it->second << "\r\n";
	else
		oss << "Connection: close\r\n";

	//TODO:(Cookie header support could go here)

	// End of header section
	for (size_t i = 0; i < setCookie.size(); ++i)
		oss << "Set-Cookie: " << setCookie[i] << "\r\n";
	// End of header section
	oss << "\r\n";
	return oss.str();
}

std::string HttpResponse::toString() const {
	// 1) Status line
	std::string out = buildStatusLine();
	// 2) Headers
	out += buildHeaders();
	// 3) Body (may be empty)
	out += body;
	return out;
}

int HttpResponse::getStatusCode() const { return statusCode; }
std::string HttpResponse::getStatusMessage() const { return statusMessage; }
std::map<std::string, std::string> HttpResponse::getHeaders() const { return headers; }
std::string HttpResponse::getBody() const { return body; }

void HttpResponse::insertInBody(const std::string& insertion,const std::string& afterTag) {
	std::size_t pos = body.find(afterTag);
	if (pos != std::string::npos)
		body.insert(pos + afterTag.length(), insertion);
	else
		body += insertion;
}

void HttpResponse::setStatusCode(int code) { 
	statusCode = code;
}

std::string HttpResponse::loadErrorPage(int errorCode) const {
	std::ostringstream filename;
	filename << "var/www/errors/" << errorCode << ".html";
	
	std::ifstream file(filename.str().c_str());
	if (file.is_open()) {
		std::string content;
		std::string line;
		while (std::getline(file, line)) {
			content += line + "\n";
		}
		file.close();
		return content;
	}
	
	// Se non riesce a caricare il file, ritorna la pagina di errore di default
	return getDefaultErrorPage(errorCode);
}

void HttpResponse::addSetCookie(const std::string& name, const std::string& value, const std::string& path, int maxAge, bool httpOnly) {
    std::ostringstream oss;
    oss << name << "=" << value;
    if (!path.empty())
        oss << "; Path=" << path;
    if (maxAge >= 0)
        oss << "; Max-Age=" << maxAge;
    if (httpOnly)
        oss << "; HttpOnly";
    setCookie.push_back(oss.str());
}

std::string HttpResponse::getDefaultErrorPage(int errorCode) const {
	std::ostringstream html;
	html << "<!DOCTYPE html>\n"
		 << "<html lang=\"en\">\n"
		 << "<head>\n"
		 << "    <meta charset=\"UTF-8\">\n"
		 << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		 << "    <title>Error " << errorCode << " - " << statusMessage << "</title>\n"
		 << "    <style>\n"
		 << "        body {\n"
		 << "            font-family: Arial, sans-serif;\n"
		 << "            background-color: #f4f4f4;\n"
		 << "            margin: 0;\n"
		 << "            padding: 20px;\n"
		 << "            text-align: center;\n"
		 << "        }\n"
		 << "        .error-container {\n"
		 << "            background-color: white;\n"
		 << "            border-radius: 10px;\n"
		 << "            padding: 40px;\n"
		 << "            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);\n"
		 << "            max-width: 600px;\n"
		 << "            margin: 0 auto;\n"
		 << "        }\n"
		 << "        h1 {\n"
		 << "            color: #e74c3c;\n"
		 << "            font-size: 48px;\n"
		 << "            margin-bottom: 10px;\n"
		 << "        }\n"
		 << "        h2 {\n"
		 << "            color: #2c3e50;\n"
		 << "            margin-bottom: 20px;\n"
		 << "        }\n"
		 << "        p {\n"
		 << "            color: #7f8c8d;\n"
		 << "            line-height: 1.6;\n"
		 << "        }\n"
		 << "    </style>\n"
		 << "</head>\n"
		 << "<body>\n"
		 << "    <div class=\"error-container\">\n"
		 << "        <h1>" << errorCode << "</h1>\n"
		 << "        <h2>" << statusMessage << "</h2>\n"
		 << "        <p>The requested resource could not be found on this server.</p>\n"
		 << "    </div>\n"
		 << "</body>\n"
		 << "</html>";
	
	return html.str();
}

void HttpResponse::setErrorResponse(int errorCode) {
	statusCode = errorCode;
	
	// Aggiorna il messaggio di stato
	const std::map<int,std::string> &phrases = getReasonPhrases();
	std::map<int,std::string>::const_iterator it = phrases.find(errorCode);
	if (it != phrases.end())
		statusMessage = it->second;
	else
		statusMessage = "Unknown Error";
	
	// Carica la pagina di errore
	std::string errorPage = loadErrorPage(errorCode);
	setBody(errorPage);
	
	// Imposta il Content-Type per HTML
	addHeader("Content-Type", "text/html; charset=utf-8");
}

void HttpResponse::setErrorResponse(int errorCode, const std::string& customMessage) {
	statusCode = errorCode;
	statusMessage = customMessage;
	
	// Carica la pagina di errore
	std::string errorPage = loadErrorPage(errorCode);
	setBody(errorPage);
	
	// Imposta il Content-Type per HTML
	addHeader("Content-Type", "text/html; charset=utf-8");
}

std::ostream& operator<<(std::ostream& os, const HttpResponse& resp) {
    os << BG_GREEN << "=== HTTP RESPONSE ===" << BG_RESET << std::endl;

    os << YELLOW << "Status Code: " << RESET << resp.getStatusCode() << std::endl;
    os << YELLOW << "Status Message: " << RESET << resp.getStatusMessage() << std::endl;

    os << CYAN << "Headers:" << RESET << std::endl;
    const std::map<std::string, std::string>& hdr = resp.getHeaders();

    for (std::map<std::string, std::string>::const_iterator it = hdr.begin(); it != hdr.end(); ++it) {
        os << "  " << PURPLE << it->first << RESET << ": " << it->second << std::endl;
    }

    os << CYAN << "Body:" << RESET << std::endl;
    os << GREEN << resp.getBody() << RESET << std::endl;

    os << BG_GREEN << "=====================" << BG_RESET << std::endl;
    return os;
}

