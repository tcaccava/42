#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
	: autoindex(false) {}

ServerConfig::ServerConfig(const ServerConfig &toCopy) {
	*this = toCopy;
}

ServerConfig::~ServerConfig() {}

ServerConfig& ServerConfig::operator=(const ServerConfig &other) {
	if (this != &other) {
		server_name = other.server_name;
		host = other.host;
		port = other.port;
		root = other.root;
		allowed_methods = other.allowed_methods;
		autoindex = other.autoindex;
		locations = other.locations;
		client_max_body_size = other.client_max_body_size;
		error_pages = other.error_pages;
		cgi_extensions = other.cgi_extensions;
	}
	return *this;
}


// Getters
const std::vector<std::string> &ServerConfig::getServerName() const { return server_name; }
const std::string &ServerConfig::getHost() const { return host; }
int ServerConfig::getPort() const { return port; }
const std::string &ServerConfig::getRoot() const { return root; }
const std::vector<std::string> &ServerConfig::getAllowedMethods() const { return allowed_methods; }
bool ServerConfig::getAutoIndex() const { return autoindex; }
int ServerConfig::getClientMaxBodySize() const { return client_max_body_size; }
const std::map<int, std::string> &ServerConfig::getErrorPages() const { return error_pages; }
const std::vector<std::string> &ServerConfig::getCgiExtensions() const { return cgi_extensions; }
const std::vector<LocationConfig> &ServerConfig::getLocations() const { return locations; }

// Setters
void ServerConfig::setServerName(const std::vector<std::string> &val) { server_name = val; }
void ServerConfig::setHost(const std::string &val) { host = val; }
void ServerConfig::setPort(int val) { port = val; }
void ServerConfig::setRoot(const std::string &val) { root = val; }
void ServerConfig::setAllowedMethods(const std::vector<std::string> &val) { allowed_methods = val; }
void ServerConfig::setAutoIndex(bool val) { autoindex = val; }
void ServerConfig::setClientMaxBodySize(int val) { client_max_body_size = val; }
void ServerConfig::setErrorPages(const std::map<int, std::string> &val) { error_pages = val; }
void ServerConfig::setCgiExtensions(const std::vector<std::string> &val) { cgi_extensions = val; }
void ServerConfig::setLocations(const std::vector<LocationConfig> &val) { locations = val; }

void ServerConfig::addServerName(const std::string &name) { server_name.push_back(name); }
void ServerConfig::addErrorPage(int code, const std::string &url) { error_pages[code] = url; }

std::ostream &operator<<(std::ostream &os, const ServerConfig &server) {
	os << "ServerConfig:\n";

	// Server Names
	os << "| Server Names: ";
	const std::vector<std::string> &names = server.getServerName();
	for (std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); ++it)
		os << *it << " ";
	os << "\n";

	// Host and Port
	os << "| Host: " << server.getHost() << "\n";
	os << "| Port: " << server.getPort() << "\n";

	// Root
	os << "| Root: " << server.getRoot() << "\n";

	// Allowed Methods
	os << "| Allowed Methods: ";
	const std::vector<std::string> &methods = server.getAllowedMethods();
	for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it)
		os << *it << " ";
	os << "\n";

	// Autoindex
	os << "| Autoindex: " << (server.getAutoIndex() ? "true" : "false") << "\n";

	// Client Max Body Size
	os << "| Client Max Body Size: " << server.getClientMaxBodySize() << "\n";

	// Error Pages
	os << "| Error Pages:\n";
	const std::map<int, std::string> &pages = server.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = pages.begin(); it != pages.end(); ++it)
		os << "  - [" << it->first << "] => " << it->second << "\n";

	// CGI Extensions
	os << "| CGI Extensions: ";
	const std::vector<std::string> &exts = server.getCgiExtensions();
	for (std::vector<std::string>::const_iterator it = exts.begin(); it != exts.end(); ++it)
		os << *it << " ";
	os << "\n";

	// Locations
	const std::vector<LocationConfig> &locs = server.getLocations();
	os << "| Locations:\n";
	for (size_t i = 0; i < locs.size(); ++i) {
		os << "|   Location " << i + 1 << ":\n";
		os << locs[i];  // usa operator<< di LocationConfig
	}

	return os;
}
