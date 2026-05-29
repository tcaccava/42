#include "Listener.hpp"

// Costruttore di default
Listener::Listener() : _sock(), _setup(false) {}

// Costruttore che riceve la configurazione del server
Listener::Listener(const ServerConfig& config)
	: _sock(), _setup(false), _config(config) {}

// Distruttore
Listener::~Listener() {}

// Funzione di debug per stampare le informazioni del listener
std::ostream& operator<<(std::ostream& o, Listener const& i)
{
		o << "Listener:" << i.getFd() << std::endl;
		o << "IP: " << i.getIp() << std::endl;
		o << "Port: " << i.getPort() << std::endl;
		return o;
}

// Crea e configura il socket di ascolto
int Listener::socketSetup(in_addr_t ip, uint16_t port)
{
	RET_IF_ERR(_sock.setup(ip, port), "Error setting up listener socket");
	RET_IF_ERR(_sock.setNonBlocking(), "Error setting listener socket to non-blocking");
	RET_IF_ERR(_sock.listen(SOMAXCONN), "Error setting listener socket to listen");
	_setup = true;
	return 0;
}

// Gestisce gli eventi sul socket di ascolto
int Listener::handle(Dispatcher::EventType event)
{
	if (!_setup) RET_ERR("Listener not setup");

	if (event == Dispatcher::READ) {
		Socket* client = _sock.accept();
		RET_IF_NULL(client, "Error accepting connection");

		int ret = client->setNonBlocking();
		if (ret == -1) {
			delete client;
			RET_ERR("Error setting client socket to non-blocking");
		}

		Connection* conn = new Connection(client, getPort(), getIp());
		if (conn == NULL) {
			delete client;
			RET_ERR("Error creating connection handler");
		}

		ret = Dispatcher::instance()->add(client->getFd(), Dispatcher::READ, conn);
		RET_IF_ERR(ret, "Error adding connection to Dispatcher");

		return 0;
	}
	if (event == Dispatcher::ERROR)
		RET_ERR("Error on listener socket");
	RET_ERR("Unknown event on listener socket");
}

int Listener::getFd() const { return _sock.getFd(); }

std::string Listener::getIp() const { return _sock.getIp(); }

int Listener::getPort() const { return _sock.getPort(); }

const ServerConfig & Listener::getConfig() const {
	return _config;
}