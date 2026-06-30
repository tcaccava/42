#include "../includes/Parsing.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace webserver {
    Location::Location()
        : path("")
        , root("")
        , index("index.html")
        , return_code("")
        , return_url("")
        , autoindex(false)
        , upload_path("")
        , cgi_dir("") {}

    ServerData::ServerData()
        : server_name("")
        , client_max_body_size(DEFAULT_CLIENT_MAX_BODY_SIZE) {}

    static const char* const DIRECTIVE_NAMES[] = {
        "server",
        "location",
        "listen",
        "server_name",
        "client_max_body_size",
        "error_page",
        "root",
        "index",
        "allowed_methods",
        "autoindex",
        "return",
        "upload_path",
        "cgi_ext",
        "cgi_dir"
    };

    // Apply built-in error pages when the server block does not define them
    static void applyDefaultErrorPages(ServerData& server) {
        if (server.error_pages.find(404) == server.error_pages.end())
            server.error_pages[404] = DEFAULT_ERROR_PAGE_404;
        if (server.error_pages.find(500) == server.error_pages.end())
            server.error_pages[500] = DEFAULT_ERROR_PAGE_500;
    }

    // Extract the directive referred to the current token name
    DirectiveType extractDirectiveByToken(const std::string& name) {
        for (size_t i = 0; i < sizeof(DIRECTIVE_NAMES) / sizeof(DIRECTIVE_NAMES[0]); i++) {
            if (name == DIRECTIVE_NAMES[i])
                return (static_cast<DirectiveType>(i));
        }

        emitParsingError(PARSE_ERR_INVALID_DIRECTIVE, "Unknown directive '" + name + "'");
        return DIRECTIVE_SERVER;
    }


    // Store a server-level directive (listen, server_name, ...) inside the current server block
    static void setServerDirectiveByType(ServerData& server, DirectiveType type, const std::vector<std::string>& tokens) {
        switch (type) {
            case DIRECTIVE_LISTEN:
                assertTokenCount(tokens, 2, "listen");
                server.listen.push_back(parseListenAddress(tokens[1]));
                break;
            case DIRECTIVE_SERVER_NAME:
                assertTokenCount(tokens, 2, "server_name");
                server.server_name = validateDirectiveToken(tokens[1], "server_name");
                break;
            case DIRECTIVE_CLIENT_MAX_BODY_SIZE:
                assertTokenCount(tokens, 2, "client_max_body_size");
                server.client_max_body_size = parseBodySizeLimit(tokens[1]);
                break;
            case DIRECTIVE_ERROR_PAGE:
                assertTokenCount(tokens, 3, "error_page");
                server.error_pages[parseHttpStatus(tokens[1], "error_page")] = validateDirectiveToken(tokens[2], "error_page");
                break;
            default:
                emitParsingError(PARSE_ERR_INVALID_DIRECTIVE, "Directive not allowed in server block");
        }
    }

    // Store a location-level directive (root, index, ...) inside the current location block
    static void setLocationDirectiveByType(Location& block, DirectiveType type, const std::vector<std::string>& tokens) {
        switch (type) {
            case DIRECTIVE_ROOT:
                assertTokenCount(tokens, 2, "root");
                block.root = validateDirectiveToken(tokens[1], "root");
                break;
            case DIRECTIVE_INDEX:
                assertTokenCount(tokens, 2, "index");
                block.index = validateDirectiveToken(tokens[1], "index");
                break;
            case DIRECTIVE_ALLOWED_METHODS:
                parseHttpMethodList(tokens, block.allowed_methods);
                break;
            case DIRECTIVE_AUTOINDEX:
                assertTokenCount(tokens, 2, "autoindex");
                block.autoindex = parseAutoindexFlag(tokens[1]);
                break;
            case DIRECTIVE_RETURN:
                assertMinTokenCount(tokens, 3, "return");
                block.return_code = removeLineTerminator(tokens[1]);
                parseHttpStatus(block.return_code, "return");
                block.return_url = validateDirectiveToken(mergeTokensToStr(tokens, 2), "return");
                break;
            case DIRECTIVE_UPLOAD_PATH:
                assertTokenCount(tokens, 2, "upload_path");
                block.upload_path = validateDirectiveToken(tokens[1], "upload_path");
                break;
            case DIRECTIVE_CGI_EXT:
                assertTokenCount(tokens, 3, "cgi_ext");
                block.cgi_extensions[parseCgiExtension(tokens[1])] = parseCgiExecutable(tokens[2]);
                break;
            case DIRECTIVE_CGI_DIR:
                assertTokenCount(tokens, 2, "cgi_dir");
                block.cgi_dir = validateDirectiveToken(tokens[1], "cgi_dir");
                break;
            default:
                emitParsingError(PARSE_ERR_INVALID_DIRECTIVE, "Directive not allowed in location block");
        }
    }

    // Handle a closing brace: leave a location, finalize a server, or reject stray '}'
    static void closeBlockScope(BlockScope& scope, ServerData& currentServer, WebserverConfig& config) {
        if (scope == SCOPE_LOCATION) {
            scope = SCOPE_SERVER;
            return;
        }

        if (scope == SCOPE_SERVER) {
            if (currentServer.listen.empty())
                emitParsingError(PARSE_ERR_MISSING_VALUE, "Server block missing 'listen' directive");
            applyDefaultErrorPages(currentServer);
            config.servers.push_back(currentServer);
            scope = SCOPE_OUTSIDE;
            return;
        }

        emitParsingError(PARSE_ERR_SYNTAX, "Unexpected closing brace outside of a block");
    }

    // Parse one preprocessed line and update the current block scope accordingly
    static void parseDirectiveLine(const std::string& line, BlockScope& scope, ServerData& currentServer, Location& currentLocation) {
        if (line.empty())
            return;

        const std::vector<std::string> tokens = tokenizeDirective(line);
        if (tokens.empty())
            return;

        const DirectiveType type = extractDirectiveByToken(tokens[0]);

        if (type == DIRECTIVE_SERVER) {
            if (scope != SCOPE_OUTSIDE)
                emitParsingError(PARSE_ERR_SYNTAX, "Nested server blocks are not allowed");
            assertTokenCount(tokens, 1, "server");
            currentServer = ServerData();
            scope = SCOPE_SERVER;
            return;
        }

        if (type == DIRECTIVE_LOCATION) {
            if (scope != SCOPE_SERVER)
                emitParsingError(PARSE_ERR_SYNTAX, "Location block must be inside a server block");
            assertTokenCount(tokens, 2, "location");
            currentLocation = Location();
            currentLocation.path = parseLocationUriPath(tokens[1]);
            currentServer.locations.push_back(currentLocation);
            scope = SCOPE_LOCATION;
            return;
        }

        if (scope == SCOPE_SERVER)
            setServerDirectiveByType(currentServer, type, tokens);
        else if (scope == SCOPE_LOCATION)
            setLocationDirectiveByType(currentServer.locations[currentServer.locations.size() - 1], type, tokens);
        else
            emitParsingError(PARSE_ERR_SYNTAX, "Directive outside of a valid block");
    }

    // Normalize a raw line, validate its terminator, and track opened/closed blocks
    std::string preprocessLine(std::string line, int& openBlockCount) {
        removeLineComment(line);
        normalizeWhitespace(line);

        if (!hasValidTokenDelimiters(line))
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid syntax: malformed pipe separators in line: " + line);

        if (line.empty())
            return (line);

        const char terminator = line[line.size() - 1];
        if (terminator != ';' && terminator != '{' && terminator != '}')
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid syntax: line must end with ';', '{' or '}': " + line);

        if (isBlockDelimiter(terminator)) {
            if (terminator == '{') {
                openBlockCount++;
            } else {
                if (openBlockCount == 0)
                    emitParsingError(PARSE_ERR_SYNTAX, "Invalid syntax: unexpected closing brace: " + line);
                openBlockCount--;
            }
            line = line.substr(0, line.size() - 1);
        }

        return (line);
    }

    // Read the config file line by line and build the final WebserverConfig structure
    WebserverConfig parseConfigFile(const std::string& filename) {
        WebserverConfig config;

        if (filename.empty())
            emitParsingError(PARSE_ERR_INVALID_CFG_FILE, "filename empty");

        std::ifstream input(filename.c_str(), std::ios::in);
        if (!input)
            emitParsingError(PARSE_ERR_INVALID_CFG_FILE, "Unable to open configuration file '" + filename + "'");

        int openBlockCount = 0;
        BlockScope scope = SCOPE_OUTSIDE;
        ServerData currentServer;
        Location currentLocation;
        std::string line;

        while (std::getline(input, line)) {
            std::string rawLine = line;
            removeLineComment(rawLine);
            normalizeWhitespace(rawLine);
            const bool closesBlock = !rawLine.empty() && rawLine[rawLine.size() - 1] == '}';

            const std::string processed = preprocessLine(line, openBlockCount);
            if (closesBlock) {
                closeBlockScope(scope, currentServer, config);
                continue;
            }

            if (processed.empty())
                continue;

            parseDirectiveLine(processed, scope, currentServer, currentLocation);
        }

        if (openBlockCount != 0) {
            std::ostringstream message;
            message << "Invalid syntax: missing closing brace(s), pending blocks: " << openBlockCount;
            emitParsingError(PARSE_ERR_SYNTAX, message.str());
        }

        if (scope != SCOPE_OUTSIDE)
            emitParsingError(PARSE_ERR_SYNTAX, "Invalid syntax: unfinished block at end of file");

        if (config.servers.empty())
            emitParsingError(PARSE_ERR_INVALID_CFG_FILE, "Configuration file contains no server blocks");

        input.close();
        return (config);
    }
}
