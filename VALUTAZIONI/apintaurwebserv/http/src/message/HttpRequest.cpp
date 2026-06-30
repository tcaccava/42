#include "message/HttpRequest.hpp"
#include "wire/HttpWireFormat.hpp"

namespace webserver {

    HttpRequest::HttpRequest()
        : method_(),
          target_(),
          path_(),
          query_(),
          version_(),
          headers_(),
          body_() {
    }

    void HttpRequest::resetFields() {
        method_.clear();
        target_.clear();
        path_.clear();
        query_.clear();
        version_.clear();
        headers_.clear();
        body_.clear();
    }

    std::string HttpRequest::getRequestHeaderValue(const std::string& name) const {
        const std::map<std::string, std::string>::const_iterator it = headers_.find(toLowerHttpAscii(name));
        if (it == headers_.end())
            return ("");
        return (it->second);
    }

    HttpRequest::~HttpRequest() {

    }
}
