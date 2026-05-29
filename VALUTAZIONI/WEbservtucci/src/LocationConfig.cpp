#include "LocationConfig.hpp"

LocationConfig::LocationConfig()
	: autoindex(false), redirect_code(-1) {}

LocationConfig::LocationConfig(const LocationConfig &toCopy) {
    *this = toCopy;
}

LocationConfig::~LocationConfig() {}

LocationConfig& LocationConfig::operator=(const LocationConfig &other) {
    if (this != &other) {
        path = other.path;
        root = other.root;
        allowed_methods = other.allowed_methods;
        autoindex = other.autoindex;
        index = other.index;
        error_pages = other.error_pages;
        cgi_extensions = other.cgi_extensions;
        index_pages = other.index_pages;
        upload_dir = other.upload_dir;
        redirect_code = other.redirect_code;
        redirect_url = other.redirect_url;
    }
    return *this;
}

// Getters
const std::string &LocationConfig::getPath() const { return path; }
const std::string &LocationConfig::getRoot() const { return root; }
const std::vector<std::string> &LocationConfig::getAllowedMethods() const { return allowed_methods; }
bool LocationConfig::getAutoIndex() const { return autoindex; }
const std::string &LocationConfig::getIndex() const { return index; }
const std::map<int, std::string> &LocationConfig::getErrorPages() const { return error_pages; }
const std::vector<std::string> &LocationConfig::getCgiExtensions() const { return cgi_extensions; }
const std::vector<std::string> &LocationConfig::getIndexPages() const { return index_pages; }
const std::string &LocationConfig::getUploadDir() const { return upload_dir; }
int LocationConfig::getRedirectCode() const { return redirect_code; }
const std::string &LocationConfig::getRedirectUrl() const { return redirect_url; }
bool LocationConfig::isEmpty() const { return path.empty(); }
bool LocationConfig::hasRedirect() const { return redirect_code > 0; }

// Setters
void LocationConfig::setPath(const std::string &val) { path = val; }
void LocationConfig::setRoot(const std::string &val) { root = val; }
void LocationConfig::setAllowedMethods(const std::vector<std::string> &val) { allowed_methods = val; }
void LocationConfig::setAutoIndex(bool val) { autoindex = val; }
void LocationConfig::setIndex(const std::string &val) { index = val; }
void LocationConfig::setErrorPages(const std::map<int, std::string> &val) { error_pages = val; }
void LocationConfig::setCgiExtensions(const std::vector<std::string> &val) { cgi_extensions = val; }
void LocationConfig::setIndexPages(const std::vector<std::string> &val) { index_pages = val; }
void LocationConfig::setUploadDir(const std::string &val) { upload_dir = val; }
void LocationConfig::setRedirectCode(int val) { redirect_code = val; }
void LocationConfig::setRedirectUrl(const std::string &val) { redirect_url = val; }

std::ostream &operator<<(std::ostream &os, const LocationConfig &loc) {
	os << "LocationConfig:\n";

	os << "| Path: " << loc.getPath() << "\n";
	os << "| Root: " << loc.getRoot() << "\n";

	// Allowed Methods
	os << "| Allowed Methods: ";
	const std::vector<std::string> &methods = loc.getAllowedMethods();
	for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it)
		os << *it << " ";
	os << "\n";

	// Autoindex
	os << "| Autoindex: " << (loc.getAutoIndex() ? "true" : "false") << "\n";

	// Index Pages
	os << "| Index Pages: ";
	const std::vector<std::string> &idxs = loc.getIndexPages();
	for (std::vector<std::string>::const_iterator it = idxs.begin(); it != idxs.end(); ++it)
		os << *it << " ";
	os << "\n";

	// Upload Directory
	os << "| Upload Directory: " << loc.getUploadDir() << "\n";

	// Redirect
	os << "| Redirect Code: " << loc.getRedirectCode() << "\n";
	os << "| Redirect URL: " << loc.getRedirectUrl() << "\n";

	// CGI Extensions
	os << "| CGI Extensions: ";
	const std::vector<std::string> &exts = loc.getCgiExtensions();
	for (std::vector<std::string>::const_iterator it = exts.begin(); it != exts.end(); ++it)
		os << *it << " ";
	os << "\n";

	// Error Pages
	os << "| Error Pages:\n";
	const std::map<int, std::string> &errors = loc.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = errors.begin(); it != errors.end(); ++it)
		os << "  - [" << it->first << "] => " << it->second << "\n";

	return os;
}

