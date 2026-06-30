#include "Client.hpp"
#include "Config.hpp"
#include "logger/includes/Logger.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {

    if (argc > 2) {
        std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
        std::cerr << "  (default: " << webserver::DEFAULT_CONFIG_FILE << ")" << std::endl;
        return (1);
    }

    const std::string configPath = (argc == 2) ? argv[1] : webserver::DEFAULT_CONFIG_FILE;

    try {
        webserver::Config::load(configPath);
        webserver::logLoadedConfig(webserver::Config::get());
        webserver::runWebServer(webserver::Config::get());
    } catch (const std::runtime_error& e) {
        Logger(webserver::ERROR) << e.what() << std::endl;
        std::cerr << e.what() << std::endl;
        webserver::LogStream::closeLogFile();
        return (1);
    }

    webserver::LogStream::closeLogFile();
    return (0);
}
