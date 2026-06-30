#include "handler/CgiHandler.hpp"
#include "Logger.hpp"
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

namespace webserver {

    std::string CgiHandler::extractPort(const ServerData& server) const {
        if (server.listen.empty())
            return ("80");
        const std::string& entry = server.listen[0];
        const std::string::size_type colonPos = entry.rfind(':');
        if (colonPos != std::string::npos)
            return (entry.substr(colonPos + 1));
        return (entry);
    }

    void CgiHandler::setupEnv(const HttpRequest& request, const ServerData& server,
                               const std::string& scriptPath,
                               std::vector<std::string>& storage,
                               std::vector<char*>& envp) const {
        storage.push_back("GATEWAY_INTERFACE=CGI/1.1");
        storage.push_back("SERVER_PROTOCOL=HTTP/1.1");
        storage.push_back("SERVER_SOFTWARE=webserv/1.0");
        storage.push_back("SERVER_NAME=" + (server.server_name.empty() ? std::string("localhost") : server.server_name));
        storage.push_back("SERVER_PORT=" + extractPort(server));
        storage.push_back("REQUEST_METHOD=" + request.getRequestMethod());
        storage.push_back("REQUEST_URI=" + request.getRequestTarget());
        storage.push_back("PATH_INFO=" + request.getRequestPath());
        storage.push_back("PATH_TRANSLATED=" + scriptPath);
        storage.push_back("SCRIPT_NAME=" + request.getRequestPath());
        storage.push_back("SCRIPT_FILENAME=" + scriptPath);
        storage.push_back("QUERY_STRING=" + request.getQueryString());

        const std::string contentType = request.getRequestHeaderValue("content-type");
        if (!contentType.empty())
            storage.push_back("CONTENT_TYPE=" + contentType);

        std::ostringstream ss;
        ss << request.getRequestBody().size();
        storage.push_back("CONTENT_LENGTH=" + ss.str());

        const std::map<std::string, std::string>& headers = request.getRequestHeaders();
        for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
            std::string name = "HTTP_";
            for (size_t i = 0; i < it->first.size(); i++) {
                const char c = it->first[i];
                name += (c == '-') ? '_' : static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            }
            storage.push_back(name + "=" + it->second);
        }

        for (size_t i = 0; i < storage.size(); i++)
            envp.push_back(const_cast<char*>(storage[i].c_str()));
        envp.push_back(NULL);
    }

    int CgiHandler::parseCgiOutput(const std::string& raw, HttpResponse& response) const {
        std::string sep = "\r\n\r\n";
        std::string::size_type sepPos = raw.find(sep);
        if (sepPos == std::string::npos) {
            sep = "\n\n";
            sepPos = raw.find(sep);
        }

        std::string headerPart;
        std::string bodyPart;

        if (sepPos == std::string::npos) {
            bodyPart = raw;
        } else {
            headerPart = raw.substr(0, sepPos);
            bodyPart = raw.substr(sepPos + sep.size());
        }

        int statusCode = 200;

        std::istringstream stream(headerPart);
        std::string line;
        while (std::getline(stream, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);
            if (line.empty())
                continue;

            const std::string::size_type colonPos = line.find(':');
            if (colonPos == std::string::npos)
                continue;

            std::string key = line.substr(0, colonPos);
            std::string val = line.substr(colonPos + 1);
            if (!val.empty() && val[0] == ' ')
                val = val.substr(1);

            std::string lkey = key;
            for (size_t i = 0; i < lkey.size(); i++)
                lkey[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(lkey[i])));

            if (lkey == "status") {
                const long code = std::strtol(val.c_str(), NULL, 10);
                if (code >= 100 && code <= 599)
                    statusCode = static_cast<int>(code);
            } else if (lkey == "content-length") {
                // skip — setBody() sets it automatically
            } else {
                response.setHeader(key, val);
            }
        }

        if (!response.hasResponseHeader("content-type"))
            response.setHeader("Content-Type", "text/html; charset=utf-8");

        response.setStatus(statusCode);
        response.setBody(bodyPart);
        return (statusCode);
    }

    int CgiHandler::execute(const HttpRequest& request, const Location& location,
                             const ServerData& server, const std::string& scriptPath,
                             const std::string& interpreter, HttpResponse& response) const {
        int stdinPipe[2];
        int stdoutPipe[2];

        if (::pipe(stdinPipe) < 0) {
            response.setStatus(500);
            response.setHeader("Content-Type", "text/plain; charset=utf-8");
            response.setBody("CGI pipe failed");
            return (500);
        }
        if (::pipe(stdoutPipe) < 0) {
            ::close(stdinPipe[0]);
            ::close(stdinPipe[1]);
            response.setStatus(500);
            response.setHeader("Content-Type", "text/plain; charset=utf-8");
            response.setBody("CGI pipe failed");
            return (500);
        }

        std::vector<std::string> envStorage;
        std::vector<char*> envp;
        setupEnv(request, server, scriptPath, envStorage, envp);

        const pid_t pid = ::fork();
        if (pid < 0) {
            ::close(stdinPipe[0]); ::close(stdinPipe[1]);
            ::close(stdoutPipe[0]); ::close(stdoutPipe[1]);
            response.setStatus(500);
            response.setHeader("Content-Type", "text/plain; charset=utf-8");
            response.setBody("CGI fork failed");
            return (500);
        }

        if (pid == 0) {
            ::close(stdinPipe[1]);
            ::close(stdoutPipe[0]);

            if (::dup2(stdinPipe[0], STDIN_FILENO) < 0) ::_exit(1);
            if (::dup2(stdoutPipe[1], STDOUT_FILENO) < 0) ::_exit(1);

            ::close(stdinPipe[0]);
            ::close(stdoutPipe[1]);

            char resolvedPath[1024];
            std::string finalScriptPath = scriptPath;
            if (::realpath(scriptPath.c_str(), resolvedPath) != NULL) {
                finalScriptPath = resolvedPath;
            }

            std::string workDir = location.cgi_dir;
            if (workDir.empty()) {
                const std::string::size_type lastSlash = finalScriptPath.rfind('/');
                workDir = (lastSlash != std::string::npos) ? finalScriptPath.substr(0, lastSlash) : ".";
            }
            ::chdir(workDir.c_str());

            char* argv[] = {
                const_cast<char*>(interpreter.c_str()),
                const_cast<char*>(finalScriptPath.c_str()),
                NULL
            };
            ::execve(interpreter.c_str(), argv, &envp[0]);
            ::_exit(1);
        }


        ::close(stdinPipe[0]);
        ::close(stdoutPipe[1]);

        const std::string& body = request.getRequestBody();
        if (!body.empty()) {
            size_t written = 0;
            while (written < body.size()) {
                const ssize_t n = ::write(stdinPipe[1], body.data() + written, body.size() - written);
                if (n <= 0) break;
                written += static_cast<size_t>(n);
            }
        }
        ::close(stdinPipe[1]);

        std::string cgiOutput;
        char buffer[4096];
        ssize_t n;
        while ((n = ::read(stdoutPipe[0], buffer, sizeof(buffer))) > 0)
            cgiOutput.append(buffer, static_cast<size_t>(n));
        ::close(stdoutPipe[0]);

        int status;
        ::waitpid(pid, &status, 0);

        Logger(DEBUG) << "CGI " << interpreter << " " << scriptPath
                      << " exit=" << WEXITSTATUS(status)
                      << " bytes=" << cgiOutput.size() << std::endl;

        return (parseCgiOutput(cgiOutput, response));
    }

}
