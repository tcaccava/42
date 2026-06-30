#include "message/HttpResponse.hpp"
#include "wire/HttpWireFormat.hpp"
#include <sstream>

namespace webserver {

    HttpResponse::HttpResponse()
        : statusCode_(200),
          reasonPhrase_("OK"),
          headers_(),
          body_() {
    }

    void HttpResponse::resetFields() {
        statusCode_ = 200;
        reasonPhrase_ = "OK";
        headers_.clear();
        body_.clear();
    }

    std::string HttpResponse::defaultReasonPhrase(int statusCode) {
        switch (statusCode) {
        case 200:
            return ("OK");
        case 201:
            return ("Created");
        case 204:
            return ("No Content");
        case 301:
            return ("Moved Permanently");
        case 400:
            return ("Bad Request");
        case 403:
            return ("Forbidden");
        case 404:
            return ("Not Found");
        case 405:
            return ("Method Not Allowed");
        case 411:
            return ("Length Required");
        case 413:
            return ("Payload Too Large");
        case 500:
            return ("Internal Server Error");
        case 502:
            return ("Bad Gateway");
        case 504:
            return ("Gateway Timeout");
        default:
            return ("");
        }
    }

    void HttpResponse::setStatus(int statusCode, const std::string& reasonPhrase) {
        statusCode_ = statusCode;
        if (!reasonPhrase.empty())
            reasonPhrase_ = reasonPhrase;
        else {
            const std::string fallback = defaultReasonPhrase(statusCode);
            reasonPhrase_ = fallback.empty() ? "Unknown" : fallback;
        }
    }

    void HttpResponse::setBody(const std::string& body) {
        body_ = body;
        std::ostringstream length;
        length << body_.size();
        headers_["content-length"] = length.str();
    }

    const std::string& HttpResponse::getResponseHeaderValue(const std::string& name) const {
        static const std::string empty;

        const std::map<std::string, std::string>::const_iterator it = headers_.find(toLowerHttpAscii(name));
        if (it == headers_.end())
            return (empty);
        return (it->second);
    }


    std::string HttpResponse::toWireFormat() const {
        std::ostringstream out;
        out << "HTTP/1.1 " << statusCode_ << " " << reasonPhrase_ << "\r\n";

        for (std::map<std::string, std::string>::const_iterator it = headers_.begin();
            it != headers_.end(); it++) {
            out << it->first << ": " << it->second << "\r\n";
        }

        out << "\r\n";
        out << body_;
        return (out.str());
    }

    HttpResponse::~HttpResponse() {}

}
