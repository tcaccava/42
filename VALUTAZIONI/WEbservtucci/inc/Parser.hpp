#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <sys/stat.h>
#include "ServerConfig.hpp"

class Parser {
private:
	std::string					filename;
	std::vector<ServerConfig>	servers;

public:
	Parser(const std::string &filename);
	~Parser();

	bool validate_server_config(const ServerConfig &server);
	std::vector<std::string> extract_server_blocks(const std::string &filename);
	std::vector<std::string> extract_location_blocks(const std::string &block);
	ServerConfig generate_server_instance(const std::string &block);
	LocationConfig generate_location_instance(const std::string &block, const ServerConfig &server);
	std::string extract_location_path(const std::string &block);

	const std::vector<ServerConfig> &getServers() const;
};

#endif
