#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "wire/HttpWireFormat.hpp"
#include <map>
#include <string>

namespace webserver {

    class HttpResponse {

        private:
            int                                 statusCode_;
            std::string                         reasonPhrase_;
            std::map<std::string, std::string>  headers_;
            std::string                         body_;

            // Return the default reason phrase for a status code
            static std::string  defaultReasonPhrase(int statusCode);

            HttpResponse(const HttpResponse& other);
            HttpResponse& operator=(const HttpResponse& other);

        public:
            // Construct a response defaulted to 200 OK with no headers or body
            HttpResponse();

            // Reset status, headers, and body back to the default 200 OK state
            void                                                resetFields();

            // Return true when a response header field exists under the given name
            inline bool                                         hasResponseHeader(const std::string& name) const { return (headers_.find(toLowerHttpAscii(name)) != headers_.end()); }

            // Serialize the response into an HTTP/1.1 wire format message
            std::string                                         toWireFormat() const;

            // Set the status code and optional reason phrase
            void                                                setStatus(int statusCode, const std::string& reasonPhrase = "");

            // Set or replace a response header field using a normalized field name
            inline void                                         setHeader(const std::string& name, const std::string& value) { headers_[toLowerHttpAscii(name)] = value; }

            // Set the response body and update Content-Length automatically
            void                                                setBody(const std::string& body);

            // Return the numeric HTTP status code
            inline int                                          getStatusCode() const { return (statusCode_); }

            // Return the status reason phrase
            inline const std::string&                           getReasonPhrase() const { return (reasonPhrase_); }

            // Return every normalized response header field
            inline const std::map<std::string, std::string>&    getResponseHeaders() const { return (headers_); }

            // Return a response header value, or an empty string when absent
            const std::string&                                  getResponseHeaderValue(const std::string& name) const;

            // Return the response message body
            inline const std::string&                           getResponseBody() const { return (body_); }

            ~HttpResponse();
    };

}

#endif
