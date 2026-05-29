#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
    : method(""), target(""), httpVersion(""), headers(), body(""), location(), file("") {}

HttpRequest::HttpRequest(const std::string &request, const ServerConfig &config) : location() {
    parse(request, config);
}

HttpRequest::HttpRequest(const HttpRequest& toCopy)
    : method(toCopy.method),
      target(toCopy.target),
      httpVersion(toCopy.httpVersion),
      headers(toCopy.headers),
      body(toCopy.body),
      location(toCopy.location),
      file(toCopy.file) {}

HttpRequest::~HttpRequest() {}

HttpRequest& HttpRequest::operator=(const HttpRequest& other) {
    if (this != &other) {
        method = other.method;
        target = other.target;
        httpVersion = other.httpVersion;
        headers = other.headers;
        body = other.body;
        location = other.location;
        file = other.file;
    }
    return *this;
}

static std::string toLower(const std::string& s) {
    std::string out = s;
    for (size_t i = 0; i < out.size(); ++i)
        out[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(out[i])));
    return out;
}

// --- Parsing principale ---
void HttpRequest::parse(const std::string& rawRequest, const ServerConfig &config) {
    size_t headerEnd = rawRequest.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        throw InvalidHttpRequestException("Malformed HTTP request: missing header-body separator");

    // --- HEADERS ---
    std::string headSection = rawRequest.substr(0, headerEnd);
    std::istringstream headStream(headSection);
    std::string line;

    if (!std::getline(headStream, line) || line.empty())
        throw InvalidHttpRequestException("Missing request line");
    if (line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);

    parseRequestLine(line);
    splitUriIntoPathAndFile();
    parseHeaders(headStream);

    // --- BODY ---
    std::string rawBody = rawRequest.substr(headerEnd + 4);
    parseBody(rawBody);

    // --- Validazioni base ---
    checkProtocol();
    checkTarget();
    checkMethod(config);
    isCgiAllowed();

    // Host obbligatorio in HTTP/1.1
    if (!hasHeader("Host"))
        throw InvalidHttpRequestException("Missing required 'Host' header");
    // Controllo finale dimensione massima
    if ((int)body.size() > config.getClientMaxBodySize())
        throw InvalidHttpRequestException("Payload Too Large");
}

// --- Parsing headers ---
void HttpRequest::parseHeaders(std::istringstream& headStream) {
    std::string line;
    while (std::getline(headStream, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        if (line.empty())
            break;
        parseHeaderLine(line);
    }
}

// --- Parsing body ---
void HttpRequest::parseBody(const std::string& rawBody) {
    if (hasHeader("Transfer-Encoding") && getHeader("Transfer-Encoding") == "chunked") {
        body = readChunkedBody(rawBody);
    } else if (hasHeader("Content-Length")) {
        int contentLength = std::atoi(getHeader("Content-Length").c_str());
        if ((int)rawBody.size() > contentLength)
            body = rawBody.substr(0, contentLength);
        else
            body = rawBody;
    } else {
        body = rawBody;
    }
}

// --- Request line ---
void HttpRequest::parseRequestLine(const std::string& requestLine) {
    std::stringstream ss(requestLine);
    ss >> method >> target >> httpVersion;

    if (method.empty() || target.empty() || httpVersion.empty())
        throw InvalidHttpRequestException("Incomplete request line");

    std::string extra;
    if (ss >> extra)
        throw InvalidHttpRequestException("Request line must contain only 3 parts");

    size_t queryPos = target.find('?');
    if (queryPos != std::string::npos) {
        std::string queryString = target.substr(queryPos + 1);
        parseQueryString(queryString);
        target = target.substr(0, queryPos);
    }
}

// --- Query string ---
void HttpRequest::parseQueryString(const std::string& queryString) {
    std::stringstream ss(queryString);
    std::string item;
    while (std::getline(ss, item, '&')) {
        if (item.empty()) continue;
        size_t delimiterPos = item.find('=');
        std::string key, value;
        if (delimiterPos != std::string::npos) {
            key = item.substr(0, delimiterPos);
            value = item.substr(delimiterPos + 1);
        } else {
            key = item;
            value = "";
        }
        queryParams[key] = value;
    }
}

// --- Header line ---
void HttpRequest::parseHeaderLine(const std::string& line) {
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos)
        throw InvalidHttpRequestException("Malformed header line: " + line);
    std::string key = trim(line.substr(0, colonPos));
    std::string value = trim(line.substr(colonPos + 1));
    if (headers.count(key))
        throw InvalidHttpRequestException("Duplicate header: " + key);
    headers[key] = value;
     // if this is the Cookie header, split into individual cookies
    if (toLower(key) == "cookie")
        parseCookieHeader(value);
}

void HttpRequest::parseCookieHeader(const std::string& line) {
    // "SID=abc123; theme=dark"
    std::string s = line;
    size_t pos = 0;
    while (pos < s.size()) {
        // skip leading spaces or ';'
        while (pos < s.size() && (s[pos] == ' ' || s[pos] == ';'))
            ++pos;
        if (pos >= s.size()) break;

        size_t eq = s.find('=', pos);
        if (eq == std::string::npos) break;
        std::string name  = trim(s.substr(pos, eq - pos));
        pos = eq + 1;

        size_t semi = s.find(';', pos);
        std::string val = (semi == std::string::npos
                           ? s.substr(pos)
                           : s.substr(pos, semi - pos));
        // trim any trailing spaces on value
        size_t end = val.find_last_not_of(" ");
        if (end != std::string::npos) val.erase(end + 1);

        cookies[name] = val;
        if (semi == std::string::npos) break;
        pos = semi + 1;
    }
}

// --- Path e file ---
void HttpRequest::splitUriIntoPathAndFile() {
    size_t lastSlash = target.rfind('/');
    if (lastSlash == std::string::npos) {
        target = "/";
        file = "";
        return;
    }
    std::string afterSlash = target.substr(lastSlash + 1);
    if (afterSlash.find('.') != std::string::npos) {
        file = afterSlash;
        target = target.substr(0, lastSlash + 1);
    } else {
        file = "";
    }
}

// --- Chunked body ---
std::string HttpRequest::readChunkedBody(const std::string& rawBody) {
    size_t pos = 0;
    std::string decodedBody;
    while (true) {
        size_t lineEnd = rawBody.find("\r\n", pos);
        if (lineEnd == std::string::npos)
            throw InvalidHttpRequestException("Malformed chunked body: missing chunk size line ending");
        std::string chunkSizeStr = rawBody.substr(pos, lineEnd - pos);
        char* endptr = NULL;
        long chunkSizeLong = std::strtol(chunkSizeStr.c_str(), &endptr, 16);
        if (endptr == chunkSizeStr.c_str() || *endptr != '\0' || chunkSizeLong < 0)
            throw InvalidHttpRequestException("Invalid chunk size: " + chunkSizeStr);
        int chunkSize = static_cast<int>(chunkSizeLong);
        pos = lineEnd + 2;
        if (chunkSize == 0)
            break;
        if (pos + chunkSize > rawBody.size())
            throw InvalidHttpRequestException("Chunk size exceeds body length");
        decodedBody += rawBody.substr(pos, chunkSize);
        pos += chunkSize;
        if (rawBody.substr(pos, 2) != "\r\n")
            throw InvalidHttpRequestException("Malformed chunked body: missing CRLF after chunk data");
        pos += 2;
    }
    return decodedBody;
}

// --- Validazioni ---
void HttpRequest::checkMethod(const ServerConfig &config) {
    // Controlla se il metodo è permesso nella location
    findMatchingLocation(config);
    if (location.isEmpty())
        throw InvalidHttpRequestException("No matching location found for URI: " + target);
    
    bool isAllowed = false;
    
    if (!location.getAllowedMethods().empty()) {
        // La location ha metodi specifici configurati
        std::vector<std::string> locMethods = location.getAllowedMethods();
        for (size_t i = 0; i < locMethods.size(); ++i) {
            if (locMethods[i] == method) {
                isAllowed = true;
                break;
            }
        }
    } else {
        // La location non ha metodi configurati, usa quelli del server
        std::vector<std::string> serverMethods = config.getAllowedMethods();
        if (!serverMethods.empty()) {
            for (size_t i = 0; i < serverMethods.size(); ++i) {
                if (serverMethods[i] == method) {
                    isAllowed = true;
                    break;
                }
            }
        } else {
            // Nessuna configurazione specifica, permetti i metodi di base
            isAllowed = (method == "GET" || method == "POST" || method == "DELETE");
        }
    }
    
    if (!isAllowed)
        throw UnsupportedMethodException(method);
}

void HttpRequest::checkTarget() const {
    if (target.empty() || target[0] != '/')
        throw InvalidHttpRequestException("Invalid or missing URI: " + target);
    if (target.size() > MAX_URI_LENGTH)
        throw InvalidHttpRequestException("URI Too Long: " + target);
}

void HttpRequest::checkProtocol() const {
    if (httpVersion != "HTTP/1.1")
        throw InvalidHttpRequestException("Unsupported HTTP version: " + httpVersion);
}

void HttpRequest::isCgiAllowed() const {
    size_t pos = target.rfind('.');
    if (pos == std::string::npos) {
        return;
    }
    
    std::string ext = target.substr(pos);
    
   
    const std::vector<std::string> &cgiExtensions = location.getCgiExtensions();
    for (size_t i = 0; i < cgiExtensions.size(); ++i) {
        if (ext == cgiExtensions[i]) {
            if (method != "GET" && method != "POST") {
                throw InvalidHttpRequestException("CGI execution denied: method not allowed");
            }
            return;
        }
    }
    return;
}

// --- Location ---
void HttpRequest::findMatchingLocation(const ServerConfig &config) {
    LocationConfig bestMatch;
    for (size_t i = 0; i < config.getLocations().size(); ++i) {
        const std::string &locPath = config.getLocations()[i].getPath();
        if (target.find(locPath) == 0 && (target.size() == locPath.size() || target[locPath.size()] == '/')) {
            bestMatch = config.getLocations()[i];
        }
    }
    setLocation(bestMatch);
}

// --- Multipart helpers ---

std::string HttpRequest::extractMultipartFilename() const {
    std::string contentType = getHeader("Content-Type");
    
    size_t boundaryPos = contentType.find("boundary=");
    if (boundaryPos == std::string::npos) {
        return "";
    }
    
    std::string boundary = "--" + contentType.substr(boundaryPos + 9);

    size_t pos = 0;
    while ((pos = body.find(boundary, pos)) != std::string::npos) {
        size_t headerStart = pos + boundary.length();
        // Salta eventuali \r\n dopo il boundary
        if (headerStart < body.size() && body[headerStart] == '\r')
            headerStart++;
        if (headerStart < body.size() && body[headerStart] == '\n')
            headerStart++;
            
        size_t headerEnd = body.find("\r\n\r\n", headerStart);
        if (headerEnd == std::string::npos) {
            break;
        }
        
        std::string headers = body.substr(headerStart, headerEnd - headerStart);

        // Cerca name="file" e filename="..."
        if (headers.find("name=\"file\"") != std::string::npos) {
            size_t fnPos = headers.find("filename=\"");
            if (fnPos != std::string::npos) {
                fnPos += 10; // lunghezza di "filename=\""
                size_t fnEnd = headers.find("\"", fnPos);
                if (fnEnd != std::string::npos) {
                    std::string filename = headers.substr(fnPos, fnEnd - fnPos);
                    return filename;
                }
            }
        }
        pos = headerEnd;
    }
    return "";
}

std::string HttpRequest::extractMultipartFileContent() const {
    std::string contentType = getHeader("Content-Type");
    size_t boundaryPos = contentType.find("boundary=");
    if (boundaryPos == std::string::npos) {
        return "";
    }

    std::string boundary = "--" + contentType.substr(boundaryPos + 9);

    size_t pos = 0;
    while ((pos = body.find(boundary, pos)) != std::string::npos) {
        size_t headerStart = pos + boundary.length();
        // Salta eventuali \r\n dopo il boundary
        if (headerStart < body.size() && body[headerStart] == '\r')
            headerStart++;
        if (headerStart < body.size() && body[headerStart] == '\n')
            headerStart++;
            
        size_t headerEnd = body.find("\r\n\r\n", headerStart);
        if (headerEnd == std::string::npos) {
            break;
        }
        
        std::string headers = body.substr(headerStart, headerEnd - headerStart);

        if (headers.find("name=\"file\"") != std::string::npos) {
            size_t contentStart = headerEnd + 4; // +4 per saltare \r\n\r\n
            
            // Trova il prossimo boundary per determinare dove finisce il contenuto
            size_t nextBoundary = body.find("\r\n" + boundary, contentStart);
            if (nextBoundary == std::string::npos) {
                // Prova senza \r\n
                nextBoundary = body.find(boundary, contentStart);
                if (nextBoundary == std::string::npos) {
                    nextBoundary = body.size();
                }
            }

            size_t contentEnd = nextBoundary;
            std::string content = body.substr(contentStart, contentEnd - contentStart);
            
            return content;
        }
        pos = headerEnd;
    }
    return "";
}

std::string HttpRequest::extractMultipartField(const std::string& fieldName) const {
    std::string contentType = getHeader("Content-Type");
    size_t boundaryPos = contentType.find("boundary=");
    if (boundaryPos == std::string::npos) return "";
    std::string boundary = "--" + contentType.substr(boundaryPos + 9);
    size_t pos = 0;
    while ((pos = body.find(boundary, pos)) != std::string::npos) {
        size_t headerStart = pos + boundary.length();
        size_t headerEnd = body.find("\r\n\r\n", headerStart);
        if (headerEnd == std::string::npos) break;
        std::string headersPart = body.substr(headerStart, headerEnd - headerStart);
        if (headersPart.find("name=\"" + fieldName + "\"") != std::string::npos) {
            size_t valueStart = headerEnd + 4;
            size_t valueEnd = body.find(boundary, valueStart);
            if (valueEnd == std::string::npos) valueEnd = body.size();
            size_t len = valueEnd - valueStart;
            if (len >= 2 && body[valueEnd - 2] == '\r' && body[valueEnd - 1] == '\n')
                len -= 2;
            return body.substr(valueStart, len);
        }
        pos = headerEnd;
    }
    return "";
}

// --- Utility ---
std::string HttpRequest::trim(const std::string& s) {
    std::string::size_type start = s.find_first_not_of(" \t");
    std::string::size_type end = s.find_last_not_of(" \t");
    if (start == std::string::npos)
        return "";
    return s.substr(start, end - start + 1);
}

void HttpRequest::debugPrint() const {
    std::cout << "=== HTTP REQUEST PARSED ===" << std::endl;
    std::cout << "Method:   " << method << std::endl;
    std::cout << "Target:   " << target << std::endl;
    std::cout << "HTTP Version: " << httpVersion << std::endl;
    std::cout << "\n-- Query Parameters --" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = queryParams.begin(); it != queryParams.end(); ++it)
        std::cout << it->first << " = " << it->second << std::endl;
    std::cout << "\n-- Headers --" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
        std::cout << it->first << ": " << it->second << std::endl;
    std::cout << "\n-- Body --" << std::endl;
    std::cout << (body.empty() ? "<empty>" : body) << std::endl;
    std::cout << "===========================" << std::endl;
}

// --- Getters/Setters ---
const std::string& HttpRequest::getMethod() const { return method; }
const std::string& HttpRequest::getTarget() const { return target; }
const std::string& HttpRequest::getFile() const { return file; }
const std::string& HttpRequest::getHttpVersion() const { return httpVersion; }
const std::map<std::string, std::string>& HttpRequest::getHeaders() const { return headers; }
const std::map<std::string, std::string>& HttpRequest::getQueryParams() const { return queryParams; }
const std::string& HttpRequest::getBody() const { return body; }
const LocationConfig& HttpRequest::getLocation() const { return location; }
void HttpRequest::setLocation(const LocationConfig& loc) { location = loc; }
void HttpRequest::setMethod(const std::string& m) { method = m; }
void HttpRequest::setTarget(const std::string& t) { target = t; }
void HttpRequest::setFile(const std::string& f) { file = f; }
void HttpRequest::setHttpVersion(const std::string& v) { httpVersion = v; }
void HttpRequest::setHeaders(const std::map<std::string, std::string>& h) { headers = h; }
const std::map<std::string,std::string>& HttpRequest::getCookies() const { return cookies; }
void HttpRequest::setQueryParams(const std::map<std::string, std::string>& qp) { queryParams = qp; }
void HttpRequest::setBody(const std::string& b) { body = b; }

bool HttpRequest::hasCookie(const std::string& name) const {
    return cookies.find(name) != cookies.end();
}

std::string HttpRequest::getCookie(const std::string& name) const {
    std::map<std::string,std::string>::const_iterator it = cookies.find(name);
    return it != cookies.end() ? it->second : "";
}

// --- Header utilities ---

bool HttpRequest::hasHeader(const std::string& key) const {
    std::string lowerKey = toLower(key);
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        if (toLower(it->first) == lowerKey)
            return true;
    }
    return false;
}

std::string HttpRequest::getHeader(const std::string& key) const {
    std::string lowerKey = toLower(key);
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        if (toLower(it->first) == lowerKey)
            return it->second;
    }
    return "";
}

bool HttpRequest::hasFile() const {
    std::string contentType = getHeader("Content-Type");
    return contentType.find("multipart/form-data") != std::string::npos && body.find("filename=\"") != std::string::npos;
}