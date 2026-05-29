#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class HttpException : public std::exception {
public:
    HttpException(const std::string& message)
        : message(message) {}

    virtual ~HttpException() throw() {}

    virtual const char* what() const throw() {
        return message.c_str();
    }

protected:
    std::string message;
};

class InvalidHttpRequestException : public HttpException {
public:
    InvalidHttpRequestException(const std::string& message)
        : HttpException("Invalid HTTP Request: " + message) {}
};

class MissingHeaderException : public HttpException {
public:
    MissingHeaderException(const std::string& header)
        : HttpException("Missing HTTP Header: " + header) {}
};

class UnsupportedMethodException : public HttpException {
public:
    UnsupportedMethodException(const std::string& method)
        : HttpException("Unsupported HTTP Method: " + method) {}
};

class InvalidHttpVersionException : public HttpException {
public:
    InvalidHttpVersionException(const std::string& version)
        : HttpException("Invalid HTTP Version: " + version) {}
};

class ServerErrorException : public HttpException {
public:
    ServerErrorException(const std::string& message)
        : HttpException("Server Error: " + message) {}
};

class SocketException : public HttpException {
public:
    SocketException(const std::string& message)
        : HttpException("Socket Error: " + message) {}
};

class ConfigurationException : public HttpException {
public:
    ConfigurationException(const std::string& message)
        : HttpException("Configuration Error: " + message) {}
};

class CgiException : public HttpException {
public:
    CgiException(const std::string& message)
        : HttpException("CGI Error: " + message) {}
};

class FileSystemException : public HttpException {
public:
    FileSystemException(const std::string& message)
        : HttpException("File System Error: " + message) {}
};

#endif
