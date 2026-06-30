#include "parser/HttpParser.hpp"
#include "wire/HttpWireFormat.hpp"

namespace webserver {

    static bool isSupportedRequestMethod(const std::string& method) {
        return (method == "GET"
            || method == "POST"
            || method == "DELETE"
            || method == "PUT"
            || method == "HEAD");
    }


    HttpParser::HttpParser(size_t maxRequestBodyBytes)
        : buffer_(),
          request_(),
          lastStatus_(HTTP_PARSE_INCOMPLETE),
          maxBodySize_(maxRequestBodyBytes),
          errorMessage_(),
          requestBodyTooLarge_(false) {
    }

    HttpParser::~HttpParser() {}

    void HttpParser::reset() {
        buffer_.clear();
        resetInProgressRequest();
    }

    void HttpParser::setMaxRequestBodyBytes(size_t byteCount) {
        maxBodySize_ = byteCount;
    }

    void HttpParser::resetInProgressRequest() {
        request_.resetFields();
        lastStatus_ = HTTP_PARSE_INCOMPLETE;
        errorMessage_.clear();
        requestBodyTooLarge_ = false;
    }

    void HttpParser::recordParseError(const std::string& message) {
        errorMessage_ = message;
        requestBodyTooLarge_ = false;
        lastStatus_ = HTTP_PARSE_ERROR;
    }

    void HttpParser::recordRequestBodyTooLargeError() {
        requestBodyTooLarge_ = true;
        errorMessage_ = "Request body exceeds client_max_body_size";
        lastStatus_ = HTTP_PARSE_ERROR;
    }

    HttpParseStatus HttpParser::appendReceivedOctets(const char* data, size_t length) {
        if (data != NULL && length > 0)
            buffer_.append(data, length);

        if (lastStatus_ == HTTP_PARSE_COMPLETE)
            resetInProgressRequest();

        if (parseCompleteMessageFromBuffer()) {
            lastStatus_ = HTTP_PARSE_COMPLETE;
            return (HTTP_PARSE_COMPLETE);
        }

        if (lastStatus_ == HTTP_PARSE_ERROR)
            return (HTTP_PARSE_ERROR);

        lastStatus_ = HTTP_PARSE_INCOMPLETE;
        return (HTTP_PARSE_INCOMPLETE);
    }

    HttpParseStatus HttpParser::appendReceivedOctets(const std::string& data) {
        return (appendReceivedOctets(data.c_str(), data.size()));
    }

    bool HttpParser::parseCompleteMessageFromBuffer() {
        const size_t headerSectionEnd = locateHeaderSectionEnd(buffer_);
        if (headerSectionEnd == std::string::npos)
            return (false);

        const size_t startLineEnd = locateLineTerminator(buffer_, 0);
        if (startLineEnd == std::string::npos)
            return (false);

        request_.resetFields();

        const std::string startLine = buffer_.substr(0, startLineEnd);
        if (!populateRequestStartLine(startLine))
            return (false);

        const std::string headerSection = buffer_.substr(startLineEnd + 2, headerSectionEnd - startLineEnd - 4);
        if (!populateHeaderSection(headerSection))
            return (false);

        size_t encodedMessageLength = 0;
        if (!populateRequestBody(headerSectionEnd, encodedMessageLength))
            return (false);

        buffer_.erase(0, encodedMessageLength);
        return (true);
    }

    bool HttpParser::populateRequestStartLine(const std::string& line) {
        const std::string trimmed = trimHttpFieldValue(line);
        if (trimmed.empty()) {
            recordParseError("Empty request line");
            return (false);
        }

        std::string method;
        std::string requestTarget;
        std::string protocolVersion;
        if (!tokenizeRequestStartLine(trimmed, method, requestTarget, protocolVersion)) {
            recordParseError("Malformed request line");
            return (false);
        }

        method = trimHttpFieldValue(method);
        requestTarget = trimHttpFieldValue(requestTarget);
        protocolVersion = trimHttpFieldValue(protocolVersion);

        if (!isSupportedRequestMethod(method)) {
            recordParseError("Unsupported HTTP method");
            return (false);
        }

        if (!validateProtocolVersion(protocolVersion, errorMessage_)) {
            recordParseError(errorMessage_);
            return (false);
        }

        if (requestTarget.empty()) {
            recordParseError("Empty request target");
            return (false);
        }

        request_.method_ = method;
        request_.target_ = requestTarget;
        request_.version_ = protocolVersion;
        parseRequestTargetUri(requestTarget, request_.path_, request_.query_);
        return (true);
    }

    bool HttpParser::populateHeaderSection(const std::string& headerSection) {
        size_t pos = 0;
        while (pos < headerSection.size()) {
            const size_t lineEnd = headerSection.find("\r\n", pos);
            if (lineEnd == std::string::npos) {
                recordParseError("Malformed header block");
                return (false);
            }

            const std::string line = headerSection.substr(pos, lineEnd - pos);
            pos = lineEnd + 2;

            if (line.empty())
                continue;

            if (!populateHeaderField(line))
                return (false);
        }

        return (true);
    }

    bool HttpParser::populateHeaderField(const std::string& line) {
        const size_t colon = line.find(':');
        if (colon == std::string::npos) {
            recordParseError("Malformed header line");
            return (false);
        }

        std::string name = trimHttpFieldValue(line.substr(0, colon));
        const std::string value = trimHttpFieldValue(line.substr(colon + 1));

        if (name.empty()) {
            recordParseError("Empty header name");
            return (false);
        }

        name = toLowerHttpAscii(name);
        if (request_.headers_.find(name) != request_.headers_.end()) {
            recordParseError("Duplicate header: " + name);
            return (false);
        }

        request_.headers_[name] = value;
        return (true);
    }

    bool HttpParser::populateRequestBody(size_t headerSectionEnd, size_t& encodedMessageLength) {
        const bool chunked = request_.hasRequestHeader("transfer-encoding")
            && headerFieldContainsToken(request_.getRequestHeaderValue("transfer-encoding"), "chunked");

        if (chunked) {
            if (request_.hasRequestHeader("content-length")) {
                recordParseError("Invalid combination of Transfer-Encoding and Content-Length");
                return (false);
            }

            std::string decodedBody;
            size_t chunkedBytesConsumed = 0;
            if (!decodeChunkedTransferEncoding(buffer_, headerSectionEnd, decodedBody, chunkedBytesConsumed, errorMessage_)) {
                if (!errorMessage_.empty()) {
                    recordParseError(errorMessage_);
                    return (false);
                }
                return (false);
            }

            if (decodedBody.size() > maxBodySize_) {
                recordRequestBodyTooLargeError();
                return (false);
            }

            request_.body_ = decodedBody;
            encodedMessageLength = headerSectionEnd + chunkedBytesConsumed;
            return (true);
        }

        size_t contentLength = 0;
        if (request_.hasRequestHeader("content-length")) {
            if (!parseContentLengthField(request_.getRequestHeaderValue("content-length"), contentLength, errorMessage_)) {
                recordParseError(errorMessage_);
                return (false);
            }
        } else {
            request_.body_.clear();
            encodedMessageLength = headerSectionEnd;
            return (true);
        }

        if (buffer_.size() < headerSectionEnd + contentLength)
            return (false);

        if (contentLength > maxBodySize_) {
            recordRequestBodyTooLargeError();
            return (false);
        }

        request_.body_.assign(buffer_, headerSectionEnd, contentLength);
        encodedMessageLength = headerSectionEnd + contentLength;
        return (true);
    }

}
