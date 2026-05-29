#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>

class LocationConfig {
	private:
		std::string					path;
		std::string					root;
		std::vector<std::string>	allowed_methods;
		bool						autoindex;
		std::string					index;
		std::map<int, std::string>	error_pages;
		std::vector<std::string>	cgi_extensions;
		std::vector<std::string>	index_pages;
		std::string					upload_dir;
		int							redirect_code;
		std::string					redirect_url;

	public:
		LocationConfig();
		LocationConfig(const LocationConfig &toCopy);
		~LocationConfig();
		LocationConfig& operator=(const LocationConfig &other);

		// Getters
		const std::string &getPath() const;
		const std::string &getRoot() const;
		const std::vector<std::string> &getAllowedMethods() const;
		bool getAutoIndex() const;
		const std::string &getIndex() const;
		const std::map<int, std::string> &getErrorPages() const;
		const std::vector<std::string> &getCgiExtensions() const;
		const std::vector<std::string> &getIndexPages() const;
		const std::string &getUploadDir() const;
		int getRedirectCode() const;
		const std::string &getRedirectUrl() const;
		bool isEmpty() const;
		bool hasRedirect() const;

		// Setters
		void setPath(const std::string &val);
		void setRoot(const std::string &val);
		void setAllowedMethods(const std::vector<std::string> &val);
		void setAutoIndex(bool val);
		void setIndex(const std::string &val);
		void setErrorPages(const std::map<int, std::string> &val);
		void setCgiExtensions(const std::vector<std::string> &val);
		void setIndexPages(const std::vector<std::string> &val);
		void setUploadDir(const std::string &val);
		void setRedirectCode(int val);
		void setRedirectUrl(const std::string &val);

};

std::ostream& operator<<(std::ostream &os, const LocationConfig &loc);

#endif
