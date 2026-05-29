#include <cstring>
#include <iostream>
#include <exception>
#include "Server.hpp"
#include "Exception.hpp"

bool isConf(const char *filename) {
    size_t len = std::strlen(filename);
    const char *suffix = ".conf";

    if (len < 6)
		return false;
    return std::strcmp(filename + len - 5, suffix) == 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "❌ Usage: " << argv[0] << " <config_file.conf>" << std::endl;
        return 1;
    }

    if (!isConf(argv[1])) {
        std::cerr << "❌ Error: config file must end with '.conf'" << std::endl;
        return 1;
    }

    try {
        Server server(argv[1]);
    } catch (const ConfigurationException &e) {
        std::cerr << "💥 Configuration error: " << e.what() << std::endl;
        return 1;
    } catch (const SocketException &e) {
        std::cerr << "💥 Socket error: " << e.what() << std::endl;
        return 1;
    } catch (const ServerErrorException &e) {
        std::cerr << "💥 Server error: " << e.what() << std::endl;
        return 1;
    } catch (const HttpException &e) {
        std::cerr << "💥 HTTP error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
