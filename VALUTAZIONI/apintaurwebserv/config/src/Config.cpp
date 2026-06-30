#include "../includes/Config.hpp"
#include "Logger.hpp"

namespace webserver {
    WebserverConfig Config::data_;

    // Dump the full parsed config tree into the log file, one entry per line
    void logLoadedConfig(const WebserverConfig& config) {
        Logger(SUCCESS) << "Loaded " << config.servers.size() << " server block(s)";

        for (size_t i = 0; i < config.servers.size(); ++i) {
            const ServerData& server = config.servers[i];

            Logger(SUCCESS) << "server " << i;

            for (size_t j = 0; j < server.listen.size(); ++j)
                Logger(SUCCESS) << "|-- listen: " << server.listen[j];

            if (!server.server_name.empty())
                Logger(SUCCESS) << "|-- server_name: " << server.server_name;

            Logger(SUCCESS) << "|-- client_max_body_size: " << server.client_max_body_size;

            for (std::map<int, std::string>::const_iterator it = server.error_pages.begin();
                 it != server.error_pages.end(); ++it) {
                Logger(SUCCESS) << "|-- error_page " << it->first << ": " << it->second;
            }

            for (size_t j = 0; j < server.locations.size(); ++j) {
                const Location& loc = server.locations[j];

                Logger(SUCCESS) << "|" << std::endl;
                Logger(SUCCESS) << "|-- location: " << loc.path;

                if (!loc.root.empty())
                    Logger(SUCCESS) << "|   |-- root: " << loc.root;
                if (!loc.index.empty())
                    Logger(SUCCESS) << "|   |-- index: " << loc.index;

                if (!loc.allowed_methods.empty()) {
                    std::string methods;
                    for (size_t k = 0; k < loc.allowed_methods.size(); ++k) {
                        if (k > 0)
                            methods += " ";
                        methods += loc.allowed_methods[k];
                    }
                    Logger(SUCCESS) << "|   |-- allowed_methods: " << methods;
                }

                Logger(SUCCESS) << "|   |-- autoindex: " << (loc.autoindex ? "on" : "off");

                if (!loc.return_code.empty())
                    Logger(SUCCESS) << "|   |-- return: " << loc.return_code << " " << loc.return_url;
                if (!loc.upload_path.empty())
                    Logger(SUCCESS) << "|   |-- upload_path: " << loc.upload_path;

                for (std::map<std::string, std::string>::const_iterator it = loc.cgi_extensions.begin();
                     it != loc.cgi_extensions.end(); ++it) {
                    Logger(SUCCESS) << "|   |-- cgi_ext " << it->first << ": " << it->second;
                }

                if (!loc.cgi_dir.empty())
                    Logger(SUCCESS) << "|   |-- cgi_dir: " << loc.cgi_dir;
            }
        }
    }

    Config::~Config() {}
}
