#ifndef HTTP_WIRE_FORMAT_HPP
#define HTTP_WIRE_FORMAT_HPP

#include <cstddef>
#include <string>

namespace webserver {

    // Remove optional linear whitespace surrounding an HTTP field value or token
    std::string     trimHttpFieldValue(const std::string& value);

    // Convert an ASCII string to lowercase for case-insensitive header comparisons
    std::string     toLowerHttpAscii(const std::string& value);

    // Return true when a comma-separated header field value contains an exact token
    bool            headerFieldContainsToken(const std::string& fieldValue, const std::string& token);

    // Parse a Content-Length header field into a non-negative byte count
    bool            parseContentLengthField(const std::string& fieldValue, size_t& byteCount, std::string& error);

    // Validate an HTTP protocol version token (HTTP/1.1)
    bool            validateProtocolVersion(const std::string& versionToken, std::string& error);

    // Return the index immediately after the header section terminator CRLFCRLF, or npos
    size_t          locateHeaderSectionEnd(const std::string& buffer);

    // Return the index of a CRLF line terminator starting at from, or npos
    size_t          locateLineTerminator(const std::string& buffer, size_t from);

    // Split a request start-line into method, request-target and protocol version
    bool            tokenizeRequestStartLine(const std::string& line, std::string& method, std::string& requestTarget, std::string& protocolVersion);

    // Split a request-target into path and query components
    void            parseRequestTargetUri(const std::string& requestTarget, std::string& path, std::string& query);

    // Decode chunked transfer coding from buffer[offset] into body and report bytes consumed
    bool            decodeChunkedTransferEncoding(const std::string& buffer, size_t offset, std::string& body, size_t& bytesConsumed, std::string& error);

}

#endif
