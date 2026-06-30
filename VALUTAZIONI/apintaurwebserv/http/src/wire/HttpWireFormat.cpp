#include "wire/HttpWireFormat.hpp"
#include <cctype>
#include <cstdlib>

namespace webserver {

    static bool isLinearWhitespaceCharacter(const char c) {
        return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
    }

    std::string trimHttpFieldValue(const std::string& value) {
        size_t start = 0;

        while (start < value.size() && isLinearWhitespaceCharacter(value[start]))
            start++;

        size_t end = value.size();
        while (end > start && isLinearWhitespaceCharacter(value[end - 1]))
            end--;

        return (value.substr(start, end - start));
    }

    std::string toLowerHttpAscii(const std::string& value) {
        std::string lower = value;
        for (size_t i = 0; i < lower.size(); i++)
            lower[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(lower[i])));
        return (lower);
    }

    bool headerFieldContainsToken(const std::string& fieldValue, const std::string& token) {
        const std::string normalizedFieldValue = toLowerHttpAscii(trimHttpFieldValue(fieldValue));
        const std::string normalizedToken = toLowerHttpAscii(token);

        size_t searchPos = 0;

        while (searchPos < normalizedFieldValue.size()) {
            const size_t tokenPos = normalizedFieldValue.find(normalizedToken, searchPos);

            if (tokenPos == std::string::npos)
                break;

            const bool validTokenStart = (tokenPos == 0 
                                            || normalizedFieldValue[tokenPos - 1] == ' '
                                            || normalizedFieldValue[tokenPos - 1] == '\t'
                                            || normalizedFieldValue[tokenPos - 1] == ',');

            const size_t tokenEndPos = tokenPos + normalizedToken.size();

            const bool validTokenEnd = (tokenEndPos >= normalizedFieldValue.size()
                                            || normalizedFieldValue[tokenEndPos] == ' '
                                            || normalizedFieldValue[tokenEndPos] == '\t'
                                            || normalizedFieldValue[tokenEndPos] == ',');

            if (validTokenStart && validTokenEnd)
                return (true);

            searchPos = tokenPos + 1;
        }

        return (false);
    }

    bool parseContentLengthField(const std::string& fieldValue, size_t& byteCount, std::string& error) {
        const std::string trimmed = trimHttpFieldValue(fieldValue);
        if (trimmed.empty()) {
            error = "Empty Content-Length header";
            return (false);
        }

        char* end = NULL;
        const long number = std::strtol(trimmed.c_str(), &end, 10);
        if (end == NULL || *end != '\0' || number < 0) {
            error = "Invalid Content-Length header";
            return (false);
        }

        byteCount = static_cast<size_t>(number);
        return (true);
    }

    bool validateProtocolVersion(const std::string& versionToken, std::string& error) {
        const std::string trimmed = trimHttpFieldValue(versionToken);
        if (trimmed.size() < 8 || trimmed.compare(0, 5, "HTTP/") != 0) {
            error = "Invalid HTTP version";
            return (false);
        }

        bool seenDot = false;
        for (size_t i = 5; i < trimmed.size(); i++) {
            const unsigned char ch = static_cast<unsigned char>(trimmed[i]);
            if (std::isdigit(ch))
                continue;
            if (trimmed[i] == '.' && !seenDot) {
                seenDot = true;
                continue;
            }
            error = "Invalid HTTP version";
            return (false);
        }

        return (true);
    }

    size_t locateHeaderSectionEnd(const std::string& buffer) {
        const size_t pos = buffer.find("\r\n\r\n");
        if (pos != std::string::npos)
            return (pos + 4);
        return (std::string::npos);
    }

    size_t locateLineTerminator(const std::string& buffer, size_t from) {
        const size_t pos = buffer.find("\r\n", from);
        if (pos != std::string::npos)
            return (pos);
        return (std::string::npos);
    }

    bool tokenizeRequestStartLine(const std::string& line, std::string& method, std::string& requestTarget, std::string& protocolVersion) {
        const size_t firstSpace = line.find(' ');
        if (firstSpace == std::string::npos)
            return (false);

        const size_t secondSpace = line.find(' ', firstSpace + 1);
        if (secondSpace == std::string::npos)
            return (false);

        method = line.substr(0, firstSpace);
        requestTarget = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
        protocolVersion = line.substr(secondSpace + 1);
        return (true);
    }

    void parseRequestTargetUri(const std::string& requestTarget, std::string& path, std::string& query) {
        const size_t question = requestTarget.find('?');
        if (question == std::string::npos) {
            path = requestTarget;
            query.clear();
            return;
        }

        path = requestTarget.substr(0, question);
        query = requestTarget.substr(question + 1);
    }

    bool decodeChunkedTransferEncoding(const std::string& buffer, size_t offset, std::string& body, size_t& bytesConsumed, std::string& error) {
        size_t pos = offset;
        body.clear();
        bytesConsumed = 0;

        while (true) {
            const size_t lineEnd = locateLineTerminator(buffer, pos);
            if (lineEnd == std::string::npos)
                return (false);

            std::string sizeLine = buffer.substr(pos, lineEnd - pos);
            const size_t extensionPos = sizeLine.find(';');
            if (extensionPos != std::string::npos)
                sizeLine = sizeLine.substr(0, extensionPos);
            sizeLine = trimHttpFieldValue(sizeLine);

            char* end = NULL;
            const long chunkSize = std::strtol(sizeLine.c_str(), &end, 16);
            if (end == NULL || *end != '\0' || chunkSize < 0) {
                error = "Invalid chunked body";
                return (false);
            }

            pos = lineEnd + 2;
            if (chunkSize == 0) {
                if (buffer.size() < pos + 2)
                    return (false);

                const size_t trailerEnd = buffer.find("\r\n\r\n", pos);
                if (trailerEnd != std::string::npos) {
                    bytesConsumed = (trailerEnd + 4) - offset;
                    return (true);
                }

                if (buffer.compare(pos, 2, "\r\n") == 0) {
                    bytesConsumed = (pos + 2) - offset;
                    return (true);
                }

                error = "Malformed chunked terminator";
                return (false);
            }

            if (buffer.size() < pos + static_cast<size_t>(chunkSize) + 2) {
                error.clear();
                return (false);
            }

            body.append(buffer, pos, static_cast<size_t>(chunkSize));
            pos += static_cast<size_t>(chunkSize);

            if (buffer.compare(pos, 2, "\r\n") != 0) {
                error = "Malformed chunked body";
                return (false);
            }

            pos += 2;
        }
    }

}
