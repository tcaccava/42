#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "LocationConfig.hpp"

class ServerConfig {
	private:
		std::vector<std::string>	server_name;
		std::string					host;
		int							port;
		std::string					root;
		std::vector<std::string>	allowed_methods;
		bool						autoindex;
		int							client_max_body_size;
		std::map<int, std::string>	error_pages;
		std::vector<std::string>	cgi_extensions;
		std::vector<LocationConfig>	locations;

	public:
		ServerConfig();
		ServerConfig(const ServerConfig &toCopy);
		~ServerConfig();
		ServerConfig& operator=(const ServerConfig &other);

		// Getters
		const std::vector<std::string> &getServerName() const;
		const std::string &getHost() const;
		int getPort() const;
		const std::string &getRoot() const;
		const std::vector<std::string> &getAllowedMethods() const;
		bool getAutoIndex() const;
		int getClientMaxBodySize() const;
		const std::map<int, std::string> &getErrorPages() const;
		const std::vector<std::string> &getCgiExtensions() const;
		const std::vector<LocationConfig> &getLocations() const;

		// Setters
		void setServerName(const std::vector<std::string> &val);
		void setHost(const std::string &val);
		void setPort(int val);
		void setRoot(const std::string &val);
		void setAllowedMethods(const std::vector<std::string> &val);
		void setAutoIndex(bool val);
		void setClientMaxBodySize(int val);
		void setErrorPages(const std::map<int, std::string> &val);
		void setCgiExtensions(const std::vector<std::string> &val);
		void setLocations(const std::vector<LocationConfig> &val);

		void addServerName(const std::string &name);
		void addErrorPage(int code, const std::string &url);
};

std::ostream& operator<<(std::ostream &os, const ServerConfig &server);

#endif
