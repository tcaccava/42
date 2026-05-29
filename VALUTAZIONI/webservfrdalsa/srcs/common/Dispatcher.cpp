#include "Dispatcher.hpp"
#include "ISocketHandler.hpp"
#include "Connection.hpp"
#include <vector>
#include <ctime>


// Costruttore: inizializza epoll e struttura dati
Dispatcher::Dispatcher() : alive(false), _epollFd(-1) {
	_handlers.clear();
}

// Distruttore: chiude epoll e rilascia gli handler
Dispatcher::~Dispatcher() {
	// delete all registered handlers
	for (std::map<int, ISocketHandler*>::iterator it = _handlers.begin();
			it != _handlers.end(); ++it) {
		if (it->second)
			delete it->second;
		it->second = NULL;
	}
	// close epoll fd
	if (_epollFd != -1) {
		::close(_epollFd);
		_epollFd = -1;
	}
	PRINT_DEBUG("Dispatcher destroyed");
}

// Operatore di output per debug
std::ostream & operator<<( std::ostream & o, Dispatcher const & i )
{
	(void)i;
	o << "The Dispatcher";
	return o;
}

// Converte l'evento in maschera epoll
uint32_t Dispatcher::getEv(EventType event) const {
	if (event == READ)
		return EPOLLIN;
	else if (event == WRITE)
		return EPOLLOUT;
	else if (event == ERROR)
		return EPOLLERR | EPOLLHUP; 
	else
		return 0;
}

// Ritorna il tipo di evento a partire dalla maschera epoll
Dispatcher::EventType Dispatcher::getEventType(uint32_t events) const {
	if (events & EPOLLIN)
		return READ;
	else if (events & EPOLLOUT)
		return WRITE;
	else if (events & (EPOLLERR | EPOLLHUP)) 
		return ERROR;
	PRINT_ERR("Unknown event type in get EventType");
	return UNKOWN;
}

// Crea l'istanza epoll
int Dispatcher::setup() {
	_epollFd = epoll_create(1);
	RET_IF_PERR(_epollFd, "Error creating epoll instance");
	return 0;
}

// Registra un nuovo file descriptor presso epoll
int Dispatcher::add(int fd, EventType ev, ISocketHandler* handler) {
	// check if setup was called
	RET_IF_ERR(_epollFd, "Error: epoll instance not created");
	// check for invalid parameters
	RET_IF_NULL(handler, "Error: handler is NULL");
	RET_IF_ERR(fd, "Error: invalid file descriptor");
	if (getEv(ev) == 0) RET_ERR("Error: invalid event mask");
	// check if fd is already registered
	if (_handlers.find(fd) != _handlers.end()) RET_ERR("Error: file descriptor already registered");

	// add fd to epoll
	struct epoll_event event;
	event.data.ptr = handler;
	event.events = getEv(ev);
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
	RET_IF_PERR(ret, "Error adding socket to epoll");
	// save the handler
	_handlers[fd] = handler;
	return 0;
}

// Modifica gli eventi associati a un file descriptor
int Dispatcher::mod(int fd, EventType ev, ISocketHandler* handler) {
	// check if setup was called
	RET_IF_ERR(_epollFd, "Error: epoll instance not created");
	// check for invalid parameters
	RET_IF_NULL(handler, "Error: handler is NULL");
	RET_IF_ERR(fd, "Error: invalid file descriptor");
	if (getEv(ev) == 0) RET_ERR("Error: invalid event mask");
	// check if fd is registered
	if (_handlers.find(fd) == _handlers.end()) RET_ERR("Error: file descriptor not registered");

	// modify fd in epoll
	struct epoll_event event;
	event.data.ptr = handler;
	event.events = getEv(ev);
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
	RET_IF_PERR(ret, "Error modifying socket in epoll");
	return 0;
}

// Rimuove un file descriptor da epoll
int Dispatcher::del(int fd) {
	// check if setup was called
	RET_IF_ERR(_epollFd, "Error: epoll instance not created");
	// check for invalid parameters
	RET_IF_ERR(fd, "Error: invalid file descriptor");
	// check if fd is registered
	if (_handlers.find(fd) == _handlers.end()) RET_ERR("Error: file descriptor not registered");

	// remove fd from epoll
	int ret = epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
	RET_IF_PERR(ret, "Error removing socket from epoll");
	// delete handler and remove it from map
	delete _handlers[fd];
	_handlers.erase(fd);
	return 0;
}

// Ciclo principale di gestione degli eventi
int Dispatcher::loop() {
	// check if setup was called
	RET_IF_ERR(_epollFd, "Error: epoll instance not created");

	time_t last_check = time(NULL);
	alive = true;
	while (alive) {
		struct epoll_event events[MAX_EVENTS];

		int nfds = epoll_wait(_epollFd, events, MAX_EVENTS, TIMEOUT);
		
		RET_IF_PERR(nfds, "Error waiting for events"); // nfds == -1 => error

		// if (nfds == 0)
		// 	PRINT_DEBUG("epoll timeout");

		for (int i = 0; i < nfds; ++i) {
			ISocketHandler* handler = static_cast<ISocketHandler*>(events[i].data.ptr);
			if (handler == NULL) {
					PRINT_ERR("Error: handler is NULL");
					continue;
			}

			EventType eventType = getEventType(events[i].events);
			if (eventType == UNKOWN) {
				PRINT_ERR("Unknown event type");
				continue;
			}

			int ret = handler->handle(eventType);
			PRINT_IF_ERR(ret, "Error handling event"); // do not exit!
		}

		time_t now = time(NULL);
		if (now - last_check > CONNECTION_TIMEOUT) {
			last_check = now;
			for (std::map<int, ISocketHandler*>::iterator it = _handlers.begin();
				it != _handlers.end(); ) {
				Connection* c = dynamic_cast<Connection*>(it->second);
				++it;
				if (c) c->checkTimeout(now);
			}
		}
	}
	return 0;
}

// Termina il loop degli eventi
void	Dispatcher::stop() {
	alive = false;
}


// Inizializzazione del puntatore singleton

Dispatcher*	Dispatcher::_instance = NULL;

// Restituisce l'istanza singleton del Dispatcher
Dispatcher* Dispatcher::instance() {
	if (_instance == NULL)
		_instance = new Dispatcher;
	return _instance;
}

