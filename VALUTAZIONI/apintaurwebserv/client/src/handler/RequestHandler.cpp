#include "handler/RequestHandler.hpp"
#include "handler/CgiHandler.hpp"
#include "Logger.hpp"
#include <cctype>
#include <cstdio>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

namespace webserver {

    const Location* RequestHandler::findLocation(const ServerData& server, const std::string& path) const {
        const Location* best = NULL;
        size_t bestLength = 0;

        for (size_t i = 0; i < server.locations.size(); i++) {
            const Location& location = server.locations[i];
            if (location.path.empty())
                continue;

            if (path.compare(0, location.path.size(), location.path) != 0)
                continue;

            if (location.path.size() > bestLength) {
                best = &location;
                bestLength = location.path.size();
            }
        }

        return (best);
    }

    bool RequestHandler::isMethodAllowed(const Location& location, const std::string& method) const {
        if (location.allowed_methods.empty())
            return (true);

        for (size_t i = 0; i < location.allowed_methods.size(); i++) {
            if (location.allowed_methods[i] == method)
                return (true);
        }

        return (false);
    }

    bool RequestHandler::readFileToString(const std::string& path, std::string& out) const {
        std::ifstream input(path.c_str(), std::ios::in | std::ios::binary);
        if (!input)
            return (false);

        std::ostringstream content;
        content << input.rdbuf();
        out = content.str();
        return (true);
    }

    bool RequestHandler::writeStringToFile(const std::string& path, const std::string& data) const {
        std::ofstream output(path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
        if (!output)
            return (false);

        output.write(data.data(), static_cast<std::streamsize>(data.size()));
        return (output.good());
    }

    bool RequestHandler::ensureDirectory(const std::string& path) const {
        struct stat info;
        if (::stat(path.c_str(), &info) == 0)
            return (S_ISDIR(info.st_mode));

        return (::mkdir(path.c_str(), 0755) == 0);
    }

    std::string RequestHandler::contentTypeForPath(const std::string& path) const {
        if (path.size() >= 5 && path.compare(path.size() - 5, 5, ".html") == 0)
            return ("text/html; charset=utf-8");
        if (path.size() >= 4 && path.compare(path.size() - 4, 4, ".css") == 0)
            return ("text/css");
        if (path.size() >= 3 && path.compare(path.size() - 3, 3, ".js") == 0)
            return ("application/javascript");
        return ("application/octet-stream");
    }

    std::string RequestHandler::resolveStaticFilePath(const Location& location, const std::string& requestPath) const {
        if (requestPath.empty() || requestPath[0] != '/' || pathContains2Dot(requestPath))
            return ("");

        if (location.root.empty())
            return ("");

        std::string relative = requestPath;
        if (location.path != "/" && location.path.size() <= requestPath.size()) {
            relative = requestPath.substr(location.path.size());
            if (relative.empty())
                relative = "/";
        }

        if (relative == "/") {
            if (!location.index.empty())
                relative = "/" + location.index;
            else
                relative = "/index.html";
        } else if (relative[relative.size() - 1] == '/') {
            if (!location.index.empty())
                relative += location.index;
            else
                relative += "index.html";
        }

        return (location.root + relative);
    }

    std::string RequestHandler::resolveUploadFilePath(const Location& location, const std::string& requestPath) const {
        if (location.upload_path.empty() || pathContains2Dot(requestPath))
            return ("");

        const std::string prefix = location.path == "/" ? "/upload/" : location.path + "/upload/";
        if (requestPath.size() <= prefix.size() || requestPath.compare(0, prefix.size(), prefix) != 0)
            return ("");

        const std::string fileName = requestPath.substr(prefix.size());
        if (fileName.empty() || fileName.find('/') != std::string::npos || fileName.find("..") != std::string::npos)
            return ("");

        return (location.upload_path + "/" + fileName);
    }

    int RequestHandler::respondPlain(HttpResponse& response, int statusCode, const std::string& body) const {
        response.setStatus(statusCode);
        response.setHeader("Content-Type", "text/plain; charset=utf-8");
        response.setBody(body);
        return (statusCode);
    }

    int RequestHandler::respondErrorPage(HttpResponse& response, const ServerData& server, int statusCode) const {
        const std::map<int, std::string>::const_iterator it = server.error_pages.find(statusCode);
        if (it != server.error_pages.end()) {
            std::string body;
            if (readFileToString(it->second, body)) {
                response.setStatus(statusCode);
                response.setHeader("Content-Type", "text/html; charset=utf-8");
                response.setBody(body);
                return (statusCode);
            }
        }

        if (statusCode == 404)
            return (respondPlain(response, 404, "Not Found"));
        else if (statusCode == 413)
            return (respondPlain(response, 413, "Payload Too Large"));
        else
            return (respondPlain(response, statusCode, "Error"));
    }

    int RequestHandler::respondStaticFile(HttpResponse& response, const std::string& filePath, bool includeBody) const {
        std::string fileContent;
        if (!readFileToString(filePath, fileContent)) {
            return (respondPlain(response, 404, "Not Found"));
        }

        response.setStatus(200);
        response.setHeader("Content-Type", contentTypeForPath(filePath));

        if (includeBody) {
            response.setBody(fileContent);
            return (200);
        }

        std::ostringstream length;
        length << fileContent.size();
        response.setHeader("Content-Length", length.str());
        return (200);
    }

    int RequestHandler::handleRedirect(const Location& location, HttpResponse& response) const {
        Logger(DEBUG) << "HTTP redirect -> " << location.return_url << std::endl;
        response.setStatus(301, "Moved Permanently");
        response.setHeader("Location", location.return_url);
        response.setHeader("Content-Length", "0");
        return (301);
    }

    int RequestHandler::handleGet(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const {
        Logger(DEBUG) << "HTTP GET " << request.getRequestPath() << " via " << location.path << std::endl;

        const std::string uploadPath = resolveUploadFilePath(location, request.getRequestPath());
        if (!uploadPath.empty()) {
            const int status = respondStaticFile(response, uploadPath, true);
            Logger(DEBUG) << "HTTP GET " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        const std::string filePath = resolveStaticFilePath(location, request.getRequestPath());
        if (filePath.empty()) {
            const int status = respondErrorPage(response, server, 404);
            Logger(DEBUG) << "HTTP GET " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        struct stat info;
        if (::stat(filePath.c_str(), &info) == 0 && S_ISREG(info.st_mode)) {
            const int status = respondStaticFile(response, filePath, true);
            Logger(DEBUG) << "HTTP GET " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        if (location.autoindex) {
            const std::string dirPath = resolveDirectoryPath(location, request.getRequestPath());
            struct stat dirInfo;
            if (!dirPath.empty() && ::stat(dirPath.c_str(), &dirInfo) == 0 && S_ISDIR(dirInfo.st_mode)) {
                const int status = respondAutoindex(response, dirPath, request.getRequestPath());
                Logger(DEBUG) << "HTTP GET " << request.getRequestPath() << " -> " << status << std::endl;
                return (status);
            }
        }

        {
            const int status = respondErrorPage(response, server, 404);
            Logger(DEBUG) << "HTTP GET " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }
    }

    int RequestHandler::handleHead(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const {
        Logger(DEBUG) << "HTTP HEAD " << request.getRequestPath() << " via " << location.path << std::endl;

        const std::string filePath = resolveStaticFilePath(location, request.getRequestPath());
        if (filePath.empty()) {
            const int status = respondErrorPage(response, server, 404);
            Logger(DEBUG) << "HTTP HEAD " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        struct stat info;
        if (::stat(filePath.c_str(), &info) != 0 || !S_ISREG(info.st_mode)) {
            const int status = respondErrorPage(response, server, 404);
            Logger(DEBUG) << "HTTP HEAD " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        const int status = respondStaticFile(response, filePath, false);
        Logger(DEBUG) << "HTTP HEAD " << request.getRequestPath() << " -> " << status << std::endl;
        return (status);
    }

    int RequestHandler::handlePost(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const {
        (void) server;
        Logger(DEBUG) << "HTTP POST " << request.getRequestPath() << " via " << location.path << std::endl;

        const std::string contentType = request.getRequestHeaderValue("content-type");
        const std::string::size_type mpPos = contentType.find("multipart/form-data");
        if (mpPos != std::string::npos) {
            const std::string::size_type bPos = contentType.find("boundary=", mpPos);
            if (bPos != std::string::npos && !location.upload_path.empty()) {
                const std::string boundary = contentType.substr(bPos + 9);
                if (!ensureDirectory(location.upload_path)) {
                    const int status = respondPlain(response, 500, "Cannot create upload directory");
                    Logger(DEBUG) << "HTTP POST " << request.getRequestPath() << " -> " << status << std::endl;
                    return (status);
                }
                std::string errMsg;
                if (!parseMultipart(request.getRequestBody(), boundary, location.upload_path, errMsg)) {
                    const int status = respondPlain(response, 400, errMsg);
                    Logger(DEBUG) << "HTTP POST " << request.getRequestPath() << " -> " << status << std::endl;
                    return (status);
                }
                const int status = respondPlain(response, 201, "Files uploaded");
                Logger(DEBUG) << "HTTP POST " << request.getRequestPath() << " -> " << status << std::endl;
                return (status);
            }
        }

        const std::string filePath = resolveUploadFilePath(location, request.getRequestPath());
        if (filePath.empty()) {
            const int status = respondPlain(response, 400, "Use POST on /upload/<filename>");
            Logger(DEBUG) << "HTTP POST " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        if (!ensureDirectory(location.upload_path)) {
            const int status = respondPlain(response, 500, "Cannot create upload directory");
            Logger(DEBUG) << "HTTP POST " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        if (!writeStringToFile(filePath, request.getRequestBody())) {
            const int status = respondPlain(response, 500, "Failed to write upload");
            Logger(DEBUG) << "HTTP POST " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        std::ostringstream message;
        message << "Created " << request.getRequestPath();
        const int status = respondPlain(response, 201, message.str());
        Logger(DEBUG) << "HTTP POST " << request.getRequestPath() << " -> " << status << std::endl;
        return (status);
    }

    int RequestHandler::handlePut(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const {
        (void) server;
        int status;

        Logger(DEBUG) << "HTTP PUT " << request.getRequestPath() << " via " << location.path << std::endl;
        Logger(DEBUG) << "HTTP PUT - Requested body: " << request.getRequestBody() << std::endl;
        const std::string filePath = resolveUploadFilePath(location, request.getRequestPath());
        if (filePath.empty()) {
            status = respondPlain(response, 400, "Use PUT on /upload/<filename>");
            Logger(DEBUG) << "HTTP PUT " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        if (!ensureDirectory(location.upload_path)) {
            status = respondPlain(response, 500, "Cannot create upload directory");
            Logger(DEBUG) << "HTTP PUT " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        if (!writeStringToFile(filePath, request.getRequestBody())) {
            status = respondPlain(response, 500, "Failed to write upload");
            Logger(DEBUG) << "HTTP PUT " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        std::ostringstream message;
        message << "Updated " << request.getRequestPath();
        status = respondPlain(response, 200, message.str());
        Logger(DEBUG) << "HTTP PUT " << request.getRequestPath() << " -> " << status << std::endl;
        return (status);
    }

    int RequestHandler::handleDelete(const HttpRequest& request, const Location& location, const ServerData& server, HttpResponse& response) const {
        Logger(DEBUG) << "HTTP DELETE " << request.getRequestPath() << " via " << location.path << std::endl;

        const std::string filePath = resolveUploadFilePath(location, request.getRequestPath());
        if (filePath.empty()) {
            const int status = respondPlain(response, 400, "Use DELETE on /upload/<filename>");
            Logger(DEBUG) << "HTTP DELETE " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        if (::unlink(filePath.c_str()) != 0) {
            const int status = respondErrorPage(response, server, 404);
            Logger(DEBUG) << "HTTP DELETE " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        const int status = respondPlain(response, 200, "Deleted");
        Logger(DEBUG) << "HTTP DELETE " << request.getRequestPath() << " -> " << status << std::endl;
        return (status);
    }

    int RequestHandler::handle(const HttpRequest& request, const ServerData& server, HttpResponse& response) const {
        Logger(DEBUG) << "HTTP " << request.getRequestMethod() << " " << request.getRequestPath() << std::endl;

        const Location* location = findLocation(server, request.getRequestPath());
        if (location == NULL) {
            const int status = respondErrorPage(response, server, 404);
            Logger(WARNING) << "HTTP " << request.getRequestMethod() << " " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        if (!location->return_code.empty() && !location->return_url.empty()) {
            const int status = handleRedirect(*location, response);
            Logger(DEBUG) << "HTTP " << request.getRequestMethod() << " " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        const std::string& method = request.getRequestMethod();

        if (!isMethodAllowed(*location, method)) {
            const int status = respondPlain(response, 405, "Method Not Allowed");
            Logger(WARNING) << "HTTP " << method << " " << request.getRequestPath() << " -> " << status << std::endl;
            return (status);
        }

        const std::string ext = extractExtension(request.getRequestPath());
        if (!ext.empty() && !location->cgi_extensions.empty()) {
            const std::map<std::string, std::string>::const_iterator cgiIt = location->cgi_extensions.find(ext);
            if (cgiIt != location->cgi_extensions.end()) {
                const int status = handleCgi(request, *location, server, cgiIt->second, response);
                Logger(DEBUG) << "HTTP CGI " << request.getRequestPath() << " -> " << status << std::endl;
                return (status);
            }
        }

        if (method == "GET") {
            return (handleGet(request, *location, server, response));
        }

        if (method == "HEAD") {
            return (handleHead(request, *location, server, response));
        }

        if (method == "POST") {
            return (handlePost(request, *location, server, response));
        }

        if (method == "PUT") {
            return (handlePut(request, *location, server, response));
        }

        if (method == "DELETE") {
            return (handleDelete(request, *location, server, response));
        }

        const int status = respondPlain(response, 405, "Method Not Allowed");
        Logger(WARNING) << "HTTP " << method << " " << request.getRequestPath() << " -> " << status << std::endl;
        return (status);
    }

    std::string RequestHandler::extractExtension(const std::string& path) {
        const std::string::size_type lastSlash = path.rfind('/');
        const std::string filename = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
        const std::string::size_type dotPos = filename.rfind('.');
        if (dotPos == std::string::npos || dotPos == 0)
            return ("");
        return (filename.substr(dotPos));
    }

    std::string RequestHandler::resolveDirectoryPath(const Location& location, const std::string& requestPath) const {
        if (requestPath.empty() || requestPath[0] != '/' || pathContains2Dot(requestPath))
            return ("");
        if (location.root.empty())
            return ("");

        std::string relative = requestPath;
        if (location.path != "/" && location.path.size() <= requestPath.size())
            relative = requestPath.substr(location.path.size());
        if (relative.empty())
            relative = "/";

        std::string dirPath = location.root + relative;
        if (dirPath[dirPath.size() - 1] != '/')
            dirPath += '/';
        return (dirPath);
    }

    int RequestHandler::respondAutoindex(HttpResponse& response, const std::string& dirPath, const std::string& requestPath) const {
        DIR* dir = ::opendir(dirPath.c_str());
        if (dir == NULL)
            return (respondPlain(response, 500, "Cannot open directory"));

        std::ostringstream html;
        html << "<!DOCTYPE html><html><head><title>Index of " << requestPath << "</title></head>"
             << "<body><h1>Index of " << requestPath << "</h1><hr><pre>";

        struct dirent* entry;
        while ((entry = ::readdir(dir)) != NULL) {
            const std::string name = entry->d_name;
            if (name == ".")
                continue;
            html << "<a href=\"" << name;
            if (entry->d_type == DT_DIR)
                html << "/";
            html << "\">" << name;
            if (entry->d_type == DT_DIR)
                html << "/";
            html << "</a>\n";
        }
        ::closedir(dir);

        html << "</pre><hr></body></html>";
        response.setStatus(200);
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        response.setBody(html.str());
        return (200);
    }

    int RequestHandler::handleCgi(const HttpRequest& request, const Location& location,
                                   const ServerData& server, const std::string& interpreter,
                                   HttpResponse& response) const {
        const std::string scriptPath = resolveStaticFilePath(location, request.getRequestPath());
        if (scriptPath.empty())
            return (respondErrorPage(response, server, 404));

        struct stat info;
        if (::stat(scriptPath.c_str(), &info) != 0 || !S_ISREG(info.st_mode))
            return (respondErrorPage(response, server, 404));

        CgiHandler cgi;
        return (cgi.execute(request, location, server, scriptPath, interpreter, response));
    }

    bool RequestHandler::parseMultipart(const std::string& body, const std::string& boundary,
                                         const std::string& uploadPath, std::string& errorOut) const {
        const std::string delim = "--" + boundary;
        const std::string finalDelim = "--" + boundary + "--";

        std::string::size_type pos = body.find(delim);
        if (pos == std::string::npos) {
            errorOut = "No boundary found";
            return (false);
        }

        while (true) {
            pos += delim.size();
            if (pos + 2 > body.size() || body[pos] != '\r' || body[pos + 1] != '\n')
                break;
            pos += 2;

            const std::string::size_type headersEnd = body.find("\r\n\r\n", pos);
            if (headersEnd == std::string::npos)
                break;

            const std::string headers = body.substr(pos, headersEnd - pos);
            pos = headersEnd + 4;

            const std::string::size_type nextBound = body.find("\r\n" + delim, pos);
            if (nextBound == std::string::npos)
                break;

            const std::string partBody = body.substr(pos, nextBound - pos);
            pos = nextBound + 2;

            std::string filename;
            const std::string::size_type dispPos = headers.find("Content-Disposition:");
            if (dispPos != std::string::npos) {
                const std::string::size_type fnPos = headers.find("filename=\"", dispPos);
                if (fnPos != std::string::npos) {
                    const std::string::size_type start = fnPos + 10;
                    const std::string::size_type end = headers.find('"', start);
                    if (end != std::string::npos)
                        filename = headers.substr(start, end - start);
                }
            }

            if (!filename.empty() && filename.find('/') == std::string::npos
                && filename.find("..") == std::string::npos) {
                if (!writeStringToFile(uploadPath + "/" + filename, partBody)) {
                    errorOut = "Failed to write " + filename;
                    return (false);
                }
            }

            if (body.compare(pos, finalDelim.size(), finalDelim) == 0)
                break;
        }

        return (true);
    }

}
