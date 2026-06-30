#ifndef PARSING_UTILITY_HPP
#define PARSING_UTILITY_HPP

#include <string>
#include <vector>

namespace webserver {
    enum ParseError {
        PARSE_ERR_INVALID_DIRECTIVE,
        PARSE_ERR_MISSING_VALUE,
        PARSE_ERR_SYNTAX,
        PARSE_ERR_INVALID_CFG_FILE
    };

    const std::string&          parseErrorMessage(ParseError error);
    void                        emitParsingError(ParseError error, const std::string& message);

    void                        removeLineComment(std::string& line);
    void                        normalizeWhitespace(std::string& line);
    bool                        hasValidTokenDelimiters(const std::string& line);
    std::vector<std::string>    tokenizeDirective(const std::string& line);
    std::string                 removeLineTerminator(std::string token);
    std::string                 mergeTokensToStr(const std::vector<std::string>& tokens, size_t offset);

    void                        assertMinTokenCount(const std::vector<std::string>& tokens, size_t min, const std::string& directive);
    void                        assertTokenCount(const std::vector<std::string>& tokens, size_t count, const std::string& directive);

    bool                        isNumericString(const std::string& value);
    bool                        isSupportedHttpMethod(const std::string& method);

    std::string                 validateDirectiveToken(const std::string& raw, const std::string& directive);
    int                         parseHttpStatus(const std::string& raw, const std::string& directive);
    std::string                 parseListenAddress(const std::string& raw);
    size_t                      parseBodySizeLimit(const std::string& raw);
    bool                        parseAutoindexFlag(const std::string& raw);
    std::string                 parseLocationUriPath(const std::string& raw);
    void                        parseHttpMethodList(const std::vector<std::string>& tokens, std::vector<std::string>& methods);
    std::string                 parseCgiExtension(const std::string& raw);

    inline std::string          parseCgiExecutable(const std::string& raw) { return (validateDirectiveToken(raw, "cgi_ext")); }
}

#endif
