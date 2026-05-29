#include "Settings.hpp"
#include "ConfigParser.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

// Istanza singleton
Settings* Settings::_instance = NULL;

// Costruttore privato
Settings::Settings() {}

// Restituisce l'unica istanza di Settings
Settings* Settings::instance() {
	if (!_instance)
		_instance = new Settings();
	return _instance;
}

// Carica la configurazione dal file indicato
int Settings::load(const std::string& path) {
	ConfigParser parser;
	if (!parser.parse(path)) {
		std::cerr << "Errore: direttiva non riconosciuta. Esco dal programma" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	_servers = parser.getServers();
	return 0;
}

// Restituisce il vettore dei server configurati
const std::vector<ServerConfig>& Settings::getServers() const {
	return _servers;
}

// Restituisce indirizzo e porta del server all'indice indicato
ServerAddress Settings::getAddress(size_t index) const {
	if (index >= _servers.size())
		throw std::runtime_error("Indice server non valido");
	return _servers[index].getAddress();
}
