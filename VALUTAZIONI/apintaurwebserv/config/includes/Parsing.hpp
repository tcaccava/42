#ifndef PARSING_HPP
#define PARSING_HPP

#include "parsingUtility.hpp"
#include <map>
#include <string>
#include <vector>

namespace webserver {
    static const size_t DEFAULT_CLIENT_MAX_BODY_SIZE = 1048576;

    static const char* const DEFAULT_ERROR_PAGE_404 = "./www/errors/404.html";
    static const char* const DEFAULT_ERROR_PAGE_500 = "./www/errors/500.html";

    enum DirectiveType {
        DIRECTIVE_SERVER,
        DIRECTIVE_LOCATION,
        DIRECTIVE_LISTEN,
        DIRECTIVE_SERVER_NAME,
        DIRECTIVE_CLIENT_MAX_BODY_SIZE,
        DIRECTIVE_ERROR_PAGE,
        DIRECTIVE_ROOT,
        DIRECTIVE_INDEX,
        DIRECTIVE_ALLOWED_METHODS,
        DIRECTIVE_AUTOINDEX,
        DIRECTIVE_RETURN,
        DIRECTIVE_UPLOAD_PATH,
        DIRECTIVE_CGI_EXT,
        DIRECTIVE_CGI_DIR
    };

    enum BlockScope {
        SCOPE_OUTSIDE,
        SCOPE_SERVER,
        SCOPE_LOCATION
    };

    struct Location {
        std::string                         path;
        std::string                         root;
        std::string                         index;
        std::string                         return_code;
        std::string                         return_url;
        std::vector<std::string>            allowed_methods;
        bool                                autoindex;
        std::string                         upload_path;
        std::map<std::string, std::string>  cgi_extensions;
        std::string                         cgi_dir;

        Location();
    };

    struct ServerData {
        std::vector<std::string>    listen;
        std::string                 server_name;
        size_t                      client_max_body_size;
        std::map<int, std::string>  error_pages;
        std::vector<Location>       locations;

        ServerData();
    };

    struct WebserverConfig {
        std::vector<ServerData>     servers;
    };

    DirectiveType   extractDirectiveByToken(const std::string& name);
    WebserverConfig parseConfigFile(const std::string& filename);
    std::string     preprocessLine(std::string line, int& openBlockCount);

    inline bool     isBlockDelimiter(char c) { return (c == '{' || c == '}'); }
}

#endif
