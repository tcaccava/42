#ifndef Dispatcher_HPP
# define Dispatcher_HPP

# include <iostream>
# include <string>
# include <map>
# include <sys/epoll.h>
# include <unistd.h>
# include "Config.hpp"

class ISocketHandler;  // Forward declaration

// Gestisce la registrazione e l'ascolto di socket con epoll

class Dispatcher
{

	/*
	Dispatcher (Singleton + Reactor using epoll)
	*/

	public:
		// Event types
		enum EventType {
			UNKOWN = 0,
			READ = 1,
			WRITE = 2,
			ERROR = 4,
		};

		~Dispatcher();

		static Dispatcher*	instance();
		int					setup();

		int					add(int fd, EventType ev, ISocketHandler* handler);
		int					mod(int fd, EventType ev, ISocketHandler* handler);
		int					del(int fd);

		int					loop();
		void				stop();

	private:
		static Dispatcher*	_instance;

		Dispatcher();
		Dispatcher( Dispatcher const & src );
		Dispatcher &		operator=( Dispatcher const & rhs );

		bool				alive;
		int					_epollFd;
		std::map<int, ISocketHandler*>	_handlers;

		uint32_t			getEv(EventType event) const;
		EventType			getEventType(uint32_t events) const;

};

std::ostream &				operator<<( std::ostream & o, Dispatcher const & i );

#endif 