#ifndef LISTENER_HPP
# define LISTENER_HPP

# include <iostream>
# include <string>
# include "ISocketHandler.hpp"
# include "Socket.hpp"
# include "Config.hpp"
# include "Settings.hpp"
# include "Dispatcher.hpp"
# include "Connection.hpp"
# include "ServerConfig.hpp"

class Listener : public ISocketHandler
{
	/*
	 * Listener socket (handler)
	 */
	public:
		Listener();
		Listener(const ServerConfig& config);
		~Listener();

		int				socketSetup(in_addr_t ip, uint16_t port);
		int				getFd() const;
		int				getPort() const;
		std::string		getIp() const;
		const ServerConfig & getConfig() const;

		virtual int		handle(Dispatcher::EventType events);

	private:
		Listener(Listener const& src);
		Listener& operator=(Listener const& rhs);

		Socket			_sock;
		bool			_setup;
		ServerConfig	_config;
};

std::ostream& operator<<(std::ostream& o, Listener const& i);

#endif
