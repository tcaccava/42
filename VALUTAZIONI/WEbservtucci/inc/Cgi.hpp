#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <sys/wait.h>
#include <sys/types.h>
#include "Exception.hpp"
#include "HttpResponse.hpp"

class Cgi {
	private:
		char** envp_;
		std::string runForkExec(const char* script);
		std::string execute(const std::string& scriptPath);

	public:
		Cgi();
		~Cgi();
		Cgi(const Cgi& src);
		Cgi& operator=(const Cgi& src);

		struct CgiSpawn {
			pid_t   pid;
			int	 read_fd;
		};
		CgiSpawn spawn(const std::string &scriptPath);
};
#endif
