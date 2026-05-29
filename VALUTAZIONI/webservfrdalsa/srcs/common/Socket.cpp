#include "Socket.hpp"

// Costruttore di default
Socket::Socket() : _fd(-1)
{
	std::memset(&_addr, 0, sizeof(_addr));
}

// Costruttore con file descriptor esistente
Socket::Socket(int fd) : _fd(fd)
{
	std::memset(&_addr, 0, sizeof(_addr));
}

// Costruttore con file descriptor e indirizzo
Socket::Socket(int fd, const struct sockaddr_in & addr) : _fd(fd), _addr(addr)
{
}

// Distruttore
Socket::~Socket() {
	int ret;
	if (ok()) {
		ret = ::close(_fd);
		PRINT_IF_PERR(ret, "Error closing socket" << _fd);
		_fd = -1; // just to be sure
	}
}

// Inizializza una socket server
int Socket::setup(in_addr_t ip, uint16_t port) {
	if (ok()) RET_ERR("Socket fd is not valid (setup)");
	// create a IPV4 TCP socket
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	RET_IF_PERR(_fd, "Error creating socket");
	// set SO_REUSEADDR to quickly restart our server after a crash
	int opt = 1;
	int ret = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	RET_IF_PERR(ret, "Error setting socket options");
	// setup socket address
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = ip;
	_addr.sin_port = htons(port);
	std::memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));
	// bind socket to address
	ret = bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr));
	RET_IF_PERR(ret, "Error binding socket");
	return 0;
}

// Mette la socket in ascolto
int Socket::listen(int backlog) {
	if (!ok()) RET_ERR("Socket fd is not valid (listen)");
	int ret = ::listen(_fd, backlog);
	RET_IF_PERR(ret, "Error listening on socket");
	return 0;
}

// Accetta una nuova connessione
Socket* Socket::accept() {
	struct sockaddr_in  _addr;
	int                 fd;

	socklen_t addrlen = sizeof(_addr);
	fd = ::accept(_fd, (struct sockaddr *)&_addr, &addrlen);
	if (fd == -1) {
		PRINT_ERR("Error accepting connection");
		return NULL;
	}
	return new Socket(fd, _addr);
}

// Connessione a un server remoto
int Socket::connect(in_addr_t ip, uint16_t port)
{
	// create socket
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	RET_IF_PERR(_fd, "Error creating socket");
	// setup socket address
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = ip;
	_addr.sin_port = htons(port);
	std::memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));
	// connect socket to address
	int ret = ::connect(_fd, (struct sockaddr *)&_addr, sizeof(_addr));
	RET_IF_PERR(ret, "Error connecting socket");
	return 0;
}

// Imposta la socket come non bloccante
int Socket::setNonBlocking() {
	if (!ok()) RET_ERR("Socket fd is not valid (setNonBlocking)");
	// get current file status flags
	int flags = fcntl(_fd, F_GETFL, 0);
	RET_IF_ERR(flags, "Error getting socket flags");
	// add O_NONBLOCK flag to current
	flags |= O_NONBLOCK;
	// set the new flags
	int ret = fcntl(_fd, F_SETFL, flags);
	RET_IF_ERR(ret, "Error setting socket to non-blocking");
	return 0;
}

// Restituisce il file descriptor
int Socket::getFd() const {
	return _fd;
}

// Restituisce l'indirizzo IP
std::string Socket::getIp() const {
	return std::string(inet_ntoa(_addr.sin_addr));
}

// Restituisce la porta
int Socket::getPort() const {
	return ntohs(_addr.sin_port);
}

// Verifica se il file descriptor è valido
bool Socket::ok() const {
	return _fd > 0;
}

// Rilascia il file descriptor senza chiuderlo
int Socket::release() {
	int fd = _fd;
	_fd = -1;
	return fd;
}

// Operatore di output per debug
std::ostream & operator<<( std::ostream & o, Socket const & i )
{
	o << "Socket fd: " << i.getFd();
	return o;
}
