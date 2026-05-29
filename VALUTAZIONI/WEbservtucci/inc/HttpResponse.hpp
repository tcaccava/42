#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

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

class HttpResponse {
	private:
		int statusCode;
		std::string statusMessage;
		std::map<std::string, std::string> headers;
		std::vector<std::string> setCookie;
		std::string body;

		static const std::map<int, std::string> &getReasonPhrases();
		std::string buildStatusLine() const;
		std::string buildHeaders()	const;
		std::string loadErrorPage(int errorCode) const;
		std::string getDefaultErrorPage(int errorCode) const;

	public:
		HttpResponse();
		HttpResponse(int code);
		HttpResponse(const HttpResponse &toCopy);
		~HttpResponse();
		HttpResponse& operator=(const HttpResponse &other);

		int getStatusCode() const;
		std::string getStatusMessage() const;
		std::map<std::string, std::string> getHeaders() const;
		std::string getBody() const;
		void setBody(const std::string &b);
		void setStatusCode(int code);
		void setErrorResponse(int errorCode);
		void setErrorResponse(int errorCode, const std::string& customMessage);
		void addHeader(const std::string &k, const std::string &v);
		std::string toString() const;
		void insertInBody(const std::string& insertion,const std::string& afterTag);
		void addSetCookie(const std::string& name, const std::string& value, const std::string& path = "/", int maxAge = -1, bool httpOnly = false);

};

std::ostream& operator<<(std::ostream& os, const HttpResponse& resp);
#endif
