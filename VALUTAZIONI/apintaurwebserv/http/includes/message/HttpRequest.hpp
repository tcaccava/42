#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "wire/HttpWireFormat.hpp"
#include <map>
#include <string>

namespace webserver {

    class HttpRequest {

        friend class HttpParser;

        private:
            std::string                         method_;
            std::string                         target_;
            std::string                         path_;
            std::string                         query_;
            std::string                         version_;
            std::map<std::string, std::string>  headers_;
            std::string                         body_;

        public:
            // Construct an empty request with no start line, headers, or body
            HttpRequest();

            // Reset every stored start line, header, and body field
            void                                                resetFields();

            // Return true when a header field exists under the given name
            inline bool                                         hasRequestHeader(const std::string& name) const { return (headers_.find(toLowerHttpAscii(name)) != headers_.end()); }

            // Return the header field value, or an empty string when absent
            std::string                                         getRequestHeaderValue(const std::string& name) const;

            // Return the HTTP method from the request start line
            inline const std::string&                           getRequestMethod() const { return (method_); }

            // Return the raw request-target from the request start line
            inline const std::string&                           getRequestTarget() const { return (target_); }

            // Return the path component extracted from the request target
            inline const std::string&                           getRequestPath() const { return (path_); }

            // Return the query string without a leading question mark
            inline const std::string&                           getQueryString() const { return (query_); }

            // Return the protocol version token from the request start line
            inline const std::string&                           getProtocolVersion() const { return (version_); }

            // Return every normalized request header field
            inline const std::map<std::string, std::string>&    getRequestHeaders() const { return (headers_); }

            // Return the decoded request message body
            inline const std::string&                           getRequestBody() const { return (body_); }

            ~HttpRequest();
    };

}

#endif
