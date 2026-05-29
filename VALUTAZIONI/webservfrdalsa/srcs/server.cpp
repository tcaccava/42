#include "Settings.hpp"
#include "Dispatcher.hpp"
#include "Socket.hpp"
#include "Listener.hpp"
#include "ServerConfig.hpp"
#include <iostream>
#include <csignal>
#include <map>
#include <sstream>
#include <arpa/inet.h>


// Gestisce i segnali di terminazione
void handleSignal(int sig) {
	(void)sig;
	if (Dispatcher* dispatcher = Dispatcher::instance())
		dispatcher->stop();
	else
		PRINT_ERR("Dispatcher non inizializzato");
}


// Inizializza un listener per l'indirizzo specificato
bool initListener(const ServerConfig& config, const ServerAddress& addr,
				  std::map<std::string, Listener*>& listeners, Dispatcher* dispatcher) {
	std::ostringstream keyStream;
	keyStream << addr.ip << ":" << addr.port;
	std::string key = keyStream.str();

	if (listeners.count(key)) {
		if (listeners[key]->getConfig().server_name == config.server_name) {
			std::cerr << "Errore, server con name, ip e porta uguali\n";
			return false;
		}
		return true;
	}

	Listener* listener = new Listener(config);
	if (listener->socketSetup(inet_addr(addr.ip.c_str()), addr.port) != 0) {
		std::cerr << "Errore setup listener su " << key << std::endl;
		delete listener;
		return false;
	}

	if (dispatcher->add(listener->getFd(), Dispatcher::READ, listener) != 0) {
		std::cerr << "Errore aggiunta listener al dispatcher\n";
		delete listener;
		return false;
	}
	
	listeners[key] = listener;
	return true;
}

// Punto di ingresso del programma server
int main(int argc, char** argv) {
	char *configPath;

	signal(SIGINT,  handleSignal);
	signal(SIGTERM, handleSignal);
	signal(SIGQUIT, handleSignal);

	char defConfigPath[] = "conf/webserv_test.conf";

	if (argc > 2)
		std::cerr << "Errore. ./webserver <conf_file>\n";
	if (argc == 2)
		configPath = argv[1];
	else
		configPath = defConfigPath;
	
	Settings* settings = Settings::instance();
	if (settings->load(configPath) != 0) {
		std::cerr << "Errore caricamento configurazione\n";
		return 1;
	}

	const std::vector<ServerConfig>& serverConfigs = settings->getServers();


	Dispatcher* dispatcher = Dispatcher::instance();
	if (dispatcher->setup() != 0) {
		std::cerr << "Errore setup dispatcher\n";
		return 1;
	}

	std::map<std::string, Listener*> listeners;
	std::cout << "server len: " << serverConfigs.size() << std::endl;
	for (size_t i = 0; i < serverConfigs.size(); ++i) {
		ServerAddress addr = settings->getAddress(i);
		std::cout << i << " Server addr " << addr.ip << " " << addr.port << std::endl;
		if (!initListener(serverConfigs[i], addr, listeners, dispatcher))
		{
			PRINT_ERR("exit");
			dispatcher->stop();
			return 1;
		}
	}

	for (std::map<std::string, Listener*>::iterator it = listeners.begin();
			it != listeners.end(); ++it) {
		std::cout << *it->second << std::endl;
	}

	return dispatcher->loop();
}
