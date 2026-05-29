#include "Request.hpp"
#include "Config.hpp"
#include <sstream>
#include <cctype>
#include <cstdlib>

// Costruttore: inizializza lo stato della richiesta
Request::Request()
	: _state(REQUEST_LINE), _contentLength(0), _chunked(false) {}

// Analizza i dati grezzi provenienti dal client
bool Request::parse(const std::string& raw) {
	_buffer += raw;
	size_t pos;

	while (_state != COMPLETE) {
		if (_state == REQUEST_LINE) {
			pos = _buffer.find("\r\n");
			if (pos == std::string::npos)
				return false;

			parseRequestLine(_buffer.substr(0, pos));
			_buffer.erase(0, pos + 2);
			_state = HEADERS;
		}
		else if (_state == HEADERS) {
			pos = _buffer.find("\r\n");
			while (pos != std::string::npos && pos != 0) {
				parseHeaderLine(_buffer.substr(0, pos));
				_buffer.erase(0, pos + 2);
				pos = _buffer.find("\r\n");
			}

			if (pos == 0) {
				_buffer.erase(0, 2); 
				std::string cl = getHeader("Content-Length");
				if (!cl.empty())
					_contentLength = std::atoi(cl.c_str());

				std::string te = getHeader("Transfer-Encoding");
				if (!te.empty()) {
					for (size_t i = 0; i < te.size(); ++i) te[i] = std::tolower(te[i]);
					if (te.find("chunked") != std::string::npos) {
							_chunked = true;
							PRINT_DEBUG("Richiesta chunked");
					}
				}

				_state = BODY;
			} else {
				return false;
			}
		}
		else if (_state == BODY) {
			if (_chunked) {
				while (true) {
					size_t crlf = _buffer.find("\r\n");
					if (crlf == std::string::npos)
						return false;
					std::string sizeStr = _buffer.substr(0, crlf);
					size_t chunkSize = std::strtoul(sizeStr.c_str(), NULL, 16);
					if (_buffer.size() < crlf + 2 + chunkSize + 2)
						return false;
					_body.append(_buffer.substr(crlf + 2, chunkSize));
					_buffer.erase(0, crlf + 2 + chunkSize + 2);
					if (chunkSize == 0) {
						if (_buffer.substr(0, 2) == "\r\n")
							_buffer.erase(0, 2);
						_state = COMPLETE;
						break;
					}
				}
			} else {
				if (_buffer.size() < _contentLength)
					return false;

				_body = _buffer.substr(0, _contentLength);
				_buffer.erase(0, _contentLength);
				_state = COMPLETE;
			}
		}
	}

	return true;
}

// Verifica se la richiesta è stata letta completamente
bool Request::isComplete() const {
	return _state == COMPLETE;
}

// Analizza la prima linea della richiesta HTTP
void Request::parseRequestLine(const std::string& line) {
	std::istringstream ss(line);
	ss >> _method >> _path >> _httpVersion;
}

// Analizza una singola riga di header
void Request::parseHeaderLine(const std::string& line) {
        size_t colon = line.find(":");
        if (colon != std::string::npos) {
                std::string key = line.substr(0, colon);
                std::string value = line.substr(colon + 1);

                while (!value.empty() && std::isspace(value[0]))
                        value.erase(0, 1);
                while (!value.empty() && std::isspace(value[value.size() - 1]))
                        value.erase(value.size() - 1);

                for (size_t i = 0; i < key.size(); ++i)
                        key[i] = std::tolower(key[i]);

                _headers[key] = value;
        }
}

// Restituisce il metodo HTTP della richiesta
const std::string& Request::getMethod() const {
	return _method;
}

// Restituisce il percorso richiesto
const std::string& Request::getPath() const {
	return _path;
}

// Restituisce la versione HTTP indicata
const std::string& Request::getHttpVersion() const {
	return _httpVersion;
}

// Restituisce la mappa degli header
const std::map<std::string, std::string>& Request::getHeaders() const {
	return _headers;
}

// Ritorna il valore di un singolo header
std::string Request::getHeader(const std::string& key) const {
        std::string lower = key;
        for (size_t i = 0; i < lower.size(); ++i)
                lower[i] = std::tolower(lower[i]);
        std::map<std::string, std::string>::const_iterator it = _headers.find(lower);
        return (it != _headers.end()) ? it->second : "";
}

// Restituisce il corpo della richiesta
const std::string& Request::getBody() const {
	return _body;
}
