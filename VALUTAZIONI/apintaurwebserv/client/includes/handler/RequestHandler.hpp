#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "Http.hpp"
#include "Parsing.hpp"
#include <string>

namespace webserver {

    class RequestHandler {

        public:
            int                 handle(const HttpRequest& request, const ServerData& server, HttpResponse& response) const;
            inline int          respondConfiguredError(HttpResponse& response, const ServerData& server, int statusCode) const { return (respondErrorPage(response, server, statusCode)); }

        private:
            static bool         pathContains2Dot(const std::string& path) { return (path.find("..") != std::string::npos); }
            static std::string  extractExtension(const std::string& path);

            const Location*     findLocation(const ServerData& server, const std::string& path) const;
            bool                isMethodAllowed(const Location& location, const std::string& method) const;

            int                 respondErrorPage(HttpResponse& response, const ServerData& server, int statusCode) const;
            int                 respondPlain(HttpResponse& response, int statusCode, const std::string& body) const;
            int                 respondStaticFile(HttpResponse& response, const std::string& filePath, bool includeBody) const;
            int                 respondAutoindex(HttpResponse& response, const std::string& dirPath, const std::string& requestPath) const;

            int                 handleGet(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const;
            int                 handleHead(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const;
            int                 handlePost(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const;
            int                 handlePut(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const;
            int                 handleDelete(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const;
            int                 handleRedirect(const Location& location, HttpResponse& response) const;
            int                 handleCgi(const HttpRequest& request, const Location& location, const ServerData& server, const std::string& interpreter, HttpResponse& response) const;

            bool                readFileToString(const std::string& path, std::string& out) const;
            bool                writeStringToFile(const std::string& path, const std::string& data) const;
            bool                ensureDirectory(const std::string& path) const;
            std::string         resolveStaticFilePath(const Location& location, const std::string& requestPath) const;
            std::string         resolveDirectoryPath(const Location& location, const std::string& requestPath) const;
            std::string         resolveUploadFilePath(const Location& location, const std::string& requestPath) const;
            std::string         contentTypeForPath(const std::string& path) const;
            bool                parseMultipart(const std::string& body, const std::string& boundary,
                                               const std::string& uploadPath, std::string& errorOut) const;
    };

}

#endif
