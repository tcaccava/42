#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <map>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

#include "Exception.hpp"
#include "ServerConfig.hpp"

#define MAX_URI_LENGTH     4096

class HttpRequest {
    private:
        std::string method;
        std::string target;
        std::string httpVersion;
        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> queryParams;
        std::map<std::string, std::string> cookies;
        std::string body;
        LocationConfig location;
        std::string file;

        // --- Parsing helpers ---
        void parseHeaders(std::istringstream& headStream);
        void parseCookieHeader(const std::string& line);
        void parseBody(const std::string& rawBody);

    public:
        HttpRequest();
        HttpRequest(const std::string &request, const ServerConfig &config);
        HttpRequest(const HttpRequest &toCopy);
        ~HttpRequest();
        HttpRequest& operator=(const HttpRequest &other);

        void parse(const std::string& rawRequest, const ServerConfig &config);
        void parseRequestLine(const std::string& line);
        void parseHeaderLine(const std::string& line);
        void parseQueryString(const std::string& queryString);
        void splitUriIntoPathAndFile();
        std::string readChunkedBody(const std::string& rawBody);
        void checkMethod(const ServerConfig &config);
        void checkTarget() const;
        void checkProtocol() const;
        void isCgiAllowed() const;
        void findMatchingLocation(const ServerConfig &config);
        std::string extractMultipartFilename() const;
        std::string extractMultipartField(const std::string& fieldName) const;
        std::string extractMultipartFileContent() const;
        std::string trim(const std::string& s);
        void debugPrint() const;

        // Getters
        const std::string& getMethod() const;
        const std::string& getTarget() const;
        const std::string& getFile() const;
        const std::string& getHttpVersion() const;
        const std::map<std::string, std::string>& getHeaders() const;
        const std::map<std::string,std::string>& getCookies() const;
        std::string getCookie(const std::string& name) const;
        const std::map<std::string, std::string>& getQueryParams() const;
        const std::string& getBody() const;
        const LocationConfig& getLocation() const;

        // Setters
        void setMethod(const std::string& m);
        void setTarget(const std::string& t);
        void setFile(const std::string& f);
        void setHttpVersion(const std::string& v);
        void setHeaders(const std::map<std::string, std::string>& h);
        void setQueryParams(const std::map<std::string, std::string>& qp);
        void setBody(const std::string& b);
        void setLocation(const LocationConfig& loc);

        // Header utilities
        bool hasHeader(const std::string& key) const;
        std::string getHeader(const std::string& key) const;
        bool hasFile() const;
        bool hasCookie(const std::string& name) const;

};

#endif