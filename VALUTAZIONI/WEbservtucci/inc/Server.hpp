#ifndef SERVER_HPP
#define SERVER_HPP

#include <cctype>
#include <string>
#include <vector>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include "Cgi.hpp"
#include "Parser.hpp"
#include "Socket.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "ServerConfig.hpp"

#define CGI_TIMEOUT 5

#define MAX_CLIENTS  FD_SETSIZE
#define BUF_SIZE     4096

/*TODO: create a struct to hold the task of the cgi*/
struct CgiTask {
    pid_t   pid;        // child's PID
    int     pipe_fd;    // read end of its stdout pipe
    int     client_fd;  // which HTTP client we're replying to
    time_t  start_time; // when we spawned it
    std::string buffer;   // accumulates CGI output
};

class Server {
	private:
		std::vector<ServerConfig>	config;
		std::vector<Socket>			clients;
		std::vector<Socket>			serverSockets;
		std::map<int, size_t>		socketToConfigIndex;
		fd_set						read_fds, write_fds;
		int							max_fd;
		std::map<int, std::string>	client_input_buffer;
		std::map<int, std::string>	client_output_buffer;
		std::map<int, size_t>		clientToConfigIndex;
		std::vector<CgiTask> cgi_tasks;


	public:
		Server(const char *file);
		Server(const Server &toCopy);
		~Server();
		Server& operator=(const Server &other);

		void	initAllServers();
		void	initClients();
		void	mainLoop();
		void	setupFdSets();
		int		waitForActivity();
		void	handleNewConnections();
		void	handleClientRequests();
		void	handleClientResponses();
		void	handleCgiJobs();
		void	closeClient(int client_fd);
		void	handleRequest(int client_fd, const std::string &data);
		const ServerConfig& getConfigForClient(int client_fd);
		void	setNonBlocking(int fd);
		void	addClient(int new_client, size_t server_index);
		HttpResponse handleGet(const HttpRequest& req, int client_fd);
		HttpResponse handleRedirect(int code, const std::string& target);
		std::string	generateRedirectHTML(int code, const std::string& target);
		HttpResponse handlePost(const HttpRequest& req);
		HttpResponse handleMultipartPost(const HttpRequest& req, const std::string& uploadDir);
		HttpResponse handleJsonPost(const HttpRequest& req, const std::string& uploadDir);
		HttpResponse handleRawPost(const HttpRequest& req, const std::string& uploadDir);
		HttpResponse handleDelete(const HttpRequest& req);
		HttpResponse autoindex(const std::string& dirPath, const HttpRequest& req);
		HttpResponse serveFile(const std::string& filePath);
		std::string getMimeType(const std::string& filePath);
		bool isScriptingFile(const std::string& filename);
		HttpResponse errorResponse(int statusCode, const std::string& debugMessage) ;
		std::string joinPath(const std::string& base, const std::string& part);
		std::string generateRandomSessionId() const;
	
		const std::vector<ServerConfig> getConfig() const;
		const std::vector<Socket> getClients() const;
		const std::vector<Socket>& getServerSockets() const;  // Cambiato da getSocket()
};

std::ostream& operator<<(std::ostream &os, const Server &ws);

#endif
