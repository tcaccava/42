#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <stdexcept>

struct LocationConfig {
	std::string path;
	std::string root;
	std::string index;
	bool autoindex;
	std::string allowed_methods;
	std::string cgi_extension;
	std::string cgi_path;
	std::string upload_store;
	std::string redirect;
	
	
	
	
	
	LocationConfig() : autoindex(false) {}
};

struct ServerAddress {
	std::string ip;
	int port;
};

struct ServerConfig {
        std::string listen;
        std::string server_name;
        std::string root;
        std::map<int, std::string> error_pages;
        int client_max_body_size;
        int cgi_timeout;
        std::vector<LocationConfig> locations;

        // listen di default su tutte le interfacce IPv4 porta 80
        ServerConfig()
                : listen("0.0.0.0:8000"), client_max_body_size(0), cgi_timeout(5) {}

	ServerAddress getAddress() const {
	ServerAddress addr;
	size_t colon = listen.find(':');
	if (colon == std::string::npos) {
		throw std::runtime_error("Invalid listen format: " + listen);
	}
	addr.ip = listen.substr(0, colon);
	std::string port_str = listen.substr(colon + 1);

	
	size_t end = port_str.find_first_not_of("0123456789");
	if (end != std::string::npos) {
		port_str = port_str.substr(0, end);
	}

	addr.port = std::atoi(port_str.c_str());

	if (addr.port <= 0 || addr.port > 65535) {
		throw std::runtime_error("Invalid port number: " + port_str);
	}

	return addr;
}
};

#endif
