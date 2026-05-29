#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

// Rappresenta e analizza una richiesta HTTP
class Request {
public:
	// Costruttore di default
	Request();

	// Analizza una stringa e popola i campi della richiesta
	bool parse(const std::string& raw);
	// Indica se la richiesta è stata completata
	bool isComplete() const;

	// Accessor per metodo, path, versione e header
	const std::string& getMethod() const;
	const std::string& getPath() const;
	const std::string& getHttpVersion() const;
	const std::map<std::string, std::string>& getHeaders() const;
	const std::string& getBody() const;
	// Restituisce un header specifico
	std::string getHeader(const std::string& key) const;

private:
	enum ParseState {
		REQUEST_LINE,
		HEADERS,
		BODY,
		COMPLETE
	};

	ParseState _state;
	std::string _method;
	std::string _path;
	std::string _httpVersion;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::string _buffer;
	size_t _contentLength;
	bool _chunked;

	// Funzioni di parsing interne
	void parseRequestLine(const std::string& line);
	void parseHeaderLine(const std::string& line);
};

#endif
