#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <vector>
#include "ServerConfig.hpp"

// Singleton che gestisce le impostazioni globali
class Settings {
public:
	// Restituisce l'istanza unica
	static Settings* instance();
	// Carica i dati di configurazione
	int load(const std::string& path);
	// Accesso alle configurazioni dei server
	const std::vector<ServerConfig>& getServers() const;
	ServerAddress getAddress(size_t index) const;

private:
	// Costruttore privato
	Settings();
	static Settings* _instance;
	std::vector<ServerConfig> _servers;
};

#endif
