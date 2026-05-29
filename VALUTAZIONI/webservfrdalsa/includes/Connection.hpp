#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <iostream>
# include <string>
# include "ISocketHandler.hpp"
# include "Socket.hpp"
# include "Config.hpp"
# include "Dispatcher.hpp"
# include "ServerConfig.hpp"
# include "Request.hpp"
# include "Response.hpp"


// Gestisce una singola connessione client
class Connection : public ISocketHandler
{
	public:
		Connection(Socket* socket, int serverPort, const std::string& serverIp);
		Connection(Socket* socket, const ServerConfig& config, int serverPort, const std::string& serverIp);
		~Connection();

		// Invocato dal Dispatcher al verificarsi di un evento
		virtual int		handle(Dispatcher::EventType events);
		// Controlla se è trascorso il timeout di inattività
		bool			checkTimeout(time_t now);

		int					getFd() const;
		std::string			getIp() const;
		int					getPort() const;

	private:
		// need socket
		Connection();
		// no copy (for socket raii)
		Connection(Connection const & src);
		Connection &			operator=(Connection const & rhs);

		// Callback interne
		int						_onRead();
		int						_onWrite();
		int						_onClose();

		// Metodi di supporto per la gestione della richiesta
		int						_handleRequest(const Request& req);
		std::string				_resolvePath(const Request& req);
		const LocationConfig*	_findBestLocation(const std::string& path) const;
		bool					_checkBodySize(const Request& req, Response& res);
		bool					_handleUpload(const Request& req, Response& res, const LocationConfig* loc);
		void					_handleCgi(const Request& req, Response& res, const LocationConfig* loc, const std::string& fullPath);
		void					_sendResponse(Response& res);

		Socket*					_sock;
		std::string				_inBuf;
		std::string				_outBuf;
		ServerConfig			_config;
		int						_serverPort;
		time_t					_lastActivity;
		std::string				_serverIp;
		bool					_configSelected; 
};

std::ostream &			operator<<( std::ostream & o, Connection const & i );

#endif 
