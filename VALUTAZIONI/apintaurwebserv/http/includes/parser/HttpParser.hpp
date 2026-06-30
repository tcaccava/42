#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include "message/HttpRequest.hpp"
#include <cstddef>
#include <string>

namespace webserver {

    /*
        [Request Line]\r\n
        [Header-1]\r\n
        [Header-2]\r\n
        ...
        [Header-N]\r\n
        \r\n
        [Body — opzionale]
    */

    // Result of the last appendReceivedOctets call on an HttpParser instance.
    enum HttpParseStatus {
        HTTP_PARSE_INCOMPLETE,
        HTTP_PARSE_COMPLETE,
        HTTP_PARSE_ERROR
    };

    class HttpParser {

        private:
            std::string         buffer_;
            HttpRequest         request_;
            HttpParseStatus     lastStatus_;
            size_t              maxBodySize_;
            std::string         errorMessage_;
            bool                requestBodyTooLarge_;

            // Clear the request in progress without discarding buffered octets
            void    resetInProgressRequest();

            // Record a fatal parse error and switch the parser to HTTP_PARSE_ERROR
            void    recordParseError(const std::string& message);

            // Record that the decoded body exceeds maxRequestBodyBytes (maps to HTTP 413)
            void    recordRequestBodyTooLargeError();

            // Parse one complete request from buffer_ when enough octets are available
            bool    parseCompleteMessageFromBuffer();

            // Populate request start-line fields from a single CRLF-terminated line
            bool    populateRequestStartLine(const std::string& line);

            // Populate all header fields from the header section without the final CRLFCRLF
            bool    populateHeaderSection(const std::string& headerSection);

            // Populate one name/value header field from a single header line
            bool    populateHeaderField(const std::string& line);

            // Populate the request body and return the total encoded message length
            bool    populateRequestBody(size_t headerSectionEnd, size_t& encodedMessageLength);

            HttpParser(const HttpParser& other);
            HttpParser& operator=(const HttpParser& other);

        public:
            // Construct a parser with an empty receive buffer and the given body size limit
            explicit HttpParser(size_t maxRequestBodyBytes);

            // Discard buffered input and reset parser state for a new connection or message
            void                        reset();

            // Set the maximum allowed decoded request body size in bytes
            void                        setMaxRequestBodyBytes(size_t byteCount);

            // Append octets from recv() and attempt to parse one complete HTTP request
            HttpParseStatus             appendReceivedOctets(const char* data, size_t length);

            // Append a string buffer and attempt to parse one complete HTTP request
            HttpParseStatus             appendReceivedOctets(const std::string& data);

            // Return the most recently parsed HTTP request
            inline const HttpRequest&   getParsedRequest() const { return (request_); }

            // Return the description recorded for the last fatal parse error
            inline const std::string&   getParseErrorMessage() const { return (errorMessage_); }

            // Return true when the last error was a client_max_body_size violation
            inline bool                 isRequestBodyTooLargeError() const { return (requestBodyTooLarge_); }

            // Return the result of the last appendReceivedOctets call
            inline HttpParseStatus      getParseStatus() const { return (lastStatus_); }

            ~HttpParser();
    };

}

#endif
