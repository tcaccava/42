#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include "Http.hpp"
#include "Parsing.hpp"
#include <string>
#include <vector>

namespace webserver {

    class CgiHandler {

        public:
            int         execute(const HttpRequest& request, const Location& location,
                                const ServerData& server, const std::string& scriptPath,
                                const std::string& interpreter, HttpResponse& response) const;

        private:
            void        setupEnv(const HttpRequest& request, const ServerData& server,
                                 const std::string& scriptPath,
                                 std::vector<std::string>& storage,
                                 std::vector<char*>& envp) const;
            int         parseCgiOutput(const std::string& raw, HttpResponse& response) const;
            std::string extractPort(const ServerData& server) const;
    };

}

#endif
