#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Exception.hpp"

# define BLUE    "\033[1;34m"
# define PURPLE  "\033[1;35m"
# define GRAY    "\033[1;30m"
# define GREEN   "\033[1;32m"
# define CYAN    "\033[1;36m"
# define YELLOW  "\033[1;33m"
# define RED     "\033[1;31m"
# define RESET   "\033[0m"
# define BG_RESET        "\033[49m"
# define BG_GREEN        "\033[42m"
# define BG_CYAN         "\033[46m"
# define BG_YELLOW       "\033[43m"
# define BG_RED          "\033[41m"

class Socket {
	private:
		int fd;

	public:
		Socket();
		Socket(const Socket &toCopy);
		~Socket();
		Socket& operator=(const Socket &other);

		void	create();
		void	bind(int port);
		void	listen();
		int		accept();
		void	closeSocket();

		int		getFd() const;
		void	setFd(int fd);
};

std::ostream& operator<<(std::ostream &os, const Socket &sock);

#endif
