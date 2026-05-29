#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <stdio.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include "Config.hpp"
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>


class Socket
{
	/*
	Socket RAII and some socket utils
	*/

	public:
		Socket();
		Socket(int fd);
		Socket(int fd, const struct sockaddr_in& addr);
		~Socket();

		// get socket fd
		int getFd() const;
		// release ownership of fd (RAII)
		int release();
		// return true if socket fd is > 0
		bool ok() const;
		// setup a IPV4 TCP socket
		int setup(in_addr_t ip, uint16_t port);
		// listen socket (for server)
		int listen(int backlog);
		// accept incoming connection (for server)
		Socket* accept();
		// Set FD as nonblocking
		int setNonBlocking();
		// getter for ip
		std::string getIp() const;
		// getter for port
		int getPort() const;
		// connect method (for client)
		int connect(in_addr_t ip, uint16_t port);

	private:
		// socket fd
		int _fd;
		// socket addr
		struct sockaddr_in _addr;
		
		// no copy is possible
		Socket & operator=( Socket const & rhs );
		Socket( Socket const & src );

};

std::ostream & operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */