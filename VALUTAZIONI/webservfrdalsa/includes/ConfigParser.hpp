#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "ServerConfig.hpp"
#include <vector>
#include <string>

// Parser per il file di configurazione del server
class ConfigParser {
public:
	// Costruttore
	ConfigParser();
	// Analizza il file indicato
	bool parse(const std::string& filepath);
	// Restituisce i server configurati
	const std::vector<ServerConfig>& getServers() const;

private:
	std::vector<ServerConfig> servers;

	// Funzioni di supporto per il parsing
	std::vector<std::string> tokenize(std::istream& file);
	bool parseServerBlock(std::vector<std::string>& tokens, size_t& index);
	bool parseLocationBlock(std::vector<std::string>& tokens, size_t& index, ServerConfig& server);
};

#endif
