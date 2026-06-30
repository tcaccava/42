#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Parsing.hpp"

namespace webserver {

    // Default configuration path when no argument is passed (subject: default path).
    static const char* const DEFAULT_CONFIG_FILE = "webserver.conf";

    class Config {
        private:
            Config();
            Config(const Config& other);
            Config& operator=(const Config& other);
            ~Config();

            static WebserverConfig          data_;

        public:
            inline static void                     load(const std::string& filename) { data_ = parseConfigFile(filename); };
            inline static const WebserverConfig&   get() { return (data_); }

    };

    void logLoadedConfig(const WebserverConfig& config);
}

#endif
