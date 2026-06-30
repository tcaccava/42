#include "../includes/parsingUtility.hpp"
#include "Logger.hpp"
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

namespace webserver {
    // Log a parsing failure and abort by throwing a runtime_error
    void emitParsingError(ParseError error, const std::string& message) {
        Logger(ERROR) << message << std::endl;
        throw std::runtime_error(parseErrorMessage(error));
    }

    // Parse a numeric token and make sure it stays within the expected range
    static long parseIntegerInRange(const std::string& raw, long min, long max, const std::string& label) {
        const std::string value = removeLineTerminator(raw);

        if (!isNumericString(value))
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid " + label + " '" + value + "'");

        char* end = NULL;
        const long number = std::strtol(value.c_str(), &end, 10);
        if (end == NULL || *end != '\0' || number < min || number > max)
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid " + label + " '" + value + "'");

        return (number);
    }

    // Return the short error label shown to the caller after a parse failure
    const std::string& parseErrorMessage(ParseError error) {
        static const std::string INVALID_DIRECTIVE = "[ERROR] : unexpected token";
        static const std::string INVALID_CFG_FILE = "[ERROR] : invalid configuration file";
        static const std::string INVALID_SYNTAX = "[ERROR] : syntax error";
        static const std::string MISSING_VALUE = "[ERROR] : missing qualifier";

        switch (error) {
            case PARSE_ERR_INVALID_CFG_FILE:
                return (INVALID_CFG_FILE);
            case PARSE_ERR_INVALID_DIRECTIVE:
                return (INVALID_DIRECTIVE);
            case PARSE_ERR_MISSING_VALUE:
                return (MISSING_VALUE);
            case PARSE_ERR_SYNTAX:
            default:
                return (INVALID_SYNTAX);
        }
    }

    // Drop everything after '#', inline comments included
    void removeLineComment(std::string& line) {
        const std::string::size_type commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line.erase(commentPos);
    }

    // Remove trailing ';', spaces and tabs left on a single token
    std::string removeLineTerminator(std::string token) {
        while (!token.empty()) {
            const char last = token[token.size() - 1];
            if (last != ';' && last != ' ' && last != '\t')
                break;
            token.erase(token.size() - 1);
        }
        return (token);
    }

    // Collapse spaces/tabs into '|' so each directive argument becomes one token
    void normalizeWhitespace(std::string& line) {
        if (line.empty())
            return;

        if (line[line.size() - 1] == '\n')
            line.erase(line.size() - 1);

        std::string normalized;
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] != ' ' && line[i] != '\t') {
                normalized += line[i];
                continue;
            }

            size_t j = i;
            while (j < line.size() && (line[j] == ' ' || line[j] == '\t'))
                j++;

            if (j >= line.size())
                break;

            if (!normalized.empty()
                && normalized[normalized.size() - 1] != '|'
                && normalized[normalized.size() - 1] != '{'
                && normalized[normalized.size() - 1] != ';'
                && line[j] != ';' && line[j] != '{' && line[j] != '}') {
                normalized += '|';
            }

            i = j - 1;
        }

        line = normalized;

        while (!line.empty() && (line[line.size() - 1] == ' ' || line[line.size() - 1] == '\t'))
            line.erase(line.size() - 1);
    }

    // Reject malformed '|' : empty fields, leading/trailing or doubled pipes
    bool hasValidTokenDelimiters(const std::string& line) {
        if (line.empty())
            return (true);

        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] != '|')
                continue;
            if (i == 0 || i == line.size() - 1)
                return (false);
            if (line[i - 1] == '|' || line[i + 1] == '|')
                return (false);
        }

        return (true);
    }

    // Split a normalized line into directive tokens using '|' as separator
    std::vector<std::string> tokenizeDirective(const std::string& line) {
        std::vector<std::string> tokens;
        std::string current;

        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] == '|') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                continue;
            }
            current += line[i];
        }

        if (!current.empty())
            tokens.push_back(current);

        return (tokens);
    }

    // Rebuild a multi-token value (e.g. to get an URL) from token[offset]
    std::string mergeTokensToStr(const std::vector<std::string>& tokens, size_t offset) {
        if (offset >= tokens.size())
            return ("");

        std::string joined = removeLineTerminator(tokens[offset]);
        for (size_t i = offset + 1; i < tokens.size(); ++i) {
            joined += " ";
            joined += removeLineTerminator(tokens[i]);
        }
        return (joined);
    }

    // Fail if the directive line has fewer tokens than required
    void assertMinTokenCount(const std::vector<std::string>& tokens, size_t min, const std::string& directive) {
        if (tokens.size() >= min)
            return;
        emitParsingError(PARSE_ERR_MISSING_VALUE, "Missing value for directive '" + directive + "'");
    }

    // Fail unless the directive line contains exactly the expected token count
    void assertTokenCount(const std::vector<std::string>& tokens, size_t count, const std::string& directive) {
        if (tokens.size() == count)
            return;

        std::ostringstream message;
        message << "Invalid syntax: directive '" << directive << "' expects exactly " << count << " token(s)";
        emitParsingError(PARSE_ERR_SYNTAX, message.str());
    }

    // True when the whole string is made of decimal digits only
    bool isNumericString(const std::string& value) {
        if (value.empty())
            return (false);

        for (size_t i = 0; i < value.size(); ++i) {
            if (!std::isdigit(static_cast<unsigned char>(value[i])))
                return (false);
        }

        return (true);
    }

    bool isSupportedHttpMethod(const std::string& method) {
        return (method == "GET"
            || method == "POST"
            || method == "DELETE"
            || method == "PUT"
            || method == "HEAD");
    }

    // Return a cleaned token or fail if the directive value is empty
    std::string validateDirectiveToken(const std::string& raw, const std::string& directive) {
        const std::string value = removeLineTerminator(raw);
        if (value.empty())
            emitParsingError(PARSE_ERR_SYNTAX, "Empty value for directive '" + directive + "'");
        return (value);
    }

    // Parse and validate an HTTP status code in the 100-599 range
    int parseHttpStatus(const std::string& raw, const std::string& directive) {
        return (static_cast<int>(parseIntegerInRange(raw, 100, 599, directive + " status code")));
    }

    // Validate a listen value: port or host:port with a valid TCP port
    std::string parseListenAddress(const std::string& raw) {
        const std::string value = removeLineTerminator(raw);
        if (value.empty())
            emitParsingError(PARSE_ERR_SYNTAX, "Empty listen value");

        const size_t colonPos = value.rfind(':');
        if (colonPos == std::string::npos) {
            parseIntegerInRange(value, 1, 65535, "listen port");
            return (value);
        }

        if (colonPos == 0 || colonPos == value.size() - 1)
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid listen value '" + value + "'");

        parseIntegerInRange(value.substr(colonPos + 1), 1, 65535, "listen port");
        return (value);
    }

    // Parse client_max_body_size with optional K/M/G suffix into bytes
    size_t parseBodySizeLimit(const std::string& raw) {
        const std::string value = removeLineTerminator(raw);
        if (value.empty())
            emitParsingError(PARSE_ERR_SYNTAX, "Empty client_max_body_size value");

        size_t digitCount = 0;
        while (digitCount < value.size() && std::isdigit(static_cast<unsigned char>(value[digitCount])))
            digitCount++;

        if (digitCount == 0)
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid client_max_body_size value '" + value + "'");

        const size_t number = std::strtoul(value.substr(0, digitCount).c_str(), NULL, 10);
        if (number == 0 && value[0] != '0')
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid client_max_body_size value '" + value + "'");

        if (digitCount == value.size())
            return (number);

        const char unit = value[digitCount];
        if (unit == 'K' || unit == 'k')
            return (number * 1024);
        if (unit == 'M' || unit == 'm')
            return (number * 1024 * 1024);
        if (unit == 'G' || unit == 'g')
            return (number * 1024 * 1024 * 1024);

        emitParsingError(PARSE_ERR_SYNTAX, "Invalid client_max_body_size suffix in '" + value + "'");
        return (number);
    }

    // Convert the autoindex flag from "on"/"off" into a bool
    bool parseAutoindexFlag(const std::string& raw) {
        const std::string value = removeLineTerminator(raw);
        if (value == "on")
            return (true);
        if (value == "off")
            return (false);
        emitParsingError(PARSE_ERR_SYNTAX, "Invalid autoindex value '" + value + "', expected 'on' or 'off'");
        return (false);
    }

    // Validate a location URI path: non-empty and must start with '/'
    std::string parseLocationUriPath(const std::string& raw) {
        const std::string path = validateDirectiveToken(raw, "location");
        if (path[0] != '/')
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid location path '" + path + "', must start with '/'");
        return (path);
    }

    // Parse allowed_methods tokens and fill the location method list
    void parseHttpMethodList(const std::vector<std::string>& tokens, std::vector<std::string>& methods) {
        assertMinTokenCount(tokens, 2, "allowed_methods");
        methods.clear();

        for (size_t i = 1; i < tokens.size(); ++i) {
            const std::string method = removeLineTerminator(tokens[i]);
            if (!isSupportedHttpMethod(method))
                emitParsingError(PARSE_ERR_SYNTAX, "Invalid HTTP method '" + method + "'");
            methods.push_back(method);
        }

        if (methods.empty())
            emitParsingError(PARSE_ERR_SYNTAX, "allowed_methods requires at least one method");
    }

    // Validate a CGI file extension token
    std::string parseCgiExtension(const std::string& raw) {
        const std::string extension = validateDirectiveToken(raw, "cgi_ext");

        if (extension.size() < 2 || extension[0] != '.')
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid cgi_ext extension '" + extension + "', expected format '.ext'");

        for (size_t i = 1; i < extension.size(); ++i) {
            const char c = extension[i];
            if (!std::isalnum(static_cast<unsigned char>(c)))
                emitParsingError(PARSE_ERR_SYNTAX, "Invalid cgi_ext extension '" + extension + "'");
        }

        return (extension);
    }

}
