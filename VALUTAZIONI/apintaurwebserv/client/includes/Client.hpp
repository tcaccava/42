#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Parsing.hpp"

namespace webserver {

    // Start the non-blocking HTTP server using one poll() for every listen and client socket.
    void                runWebServer(const WebserverConfig& config);

}

#endif
