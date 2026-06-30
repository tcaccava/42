#ifndef SOCKET_IO_HPP
#define SOCKET_IO_HPP

#include <cerrno>
#include <cstring>
#include <string>
#include <sys/types.h>

namespace webserver {

    struct ListenEndpoint {
        std::string host;
        int         port;
    };

    // Parse a listen directive value into host and TCP port.
    bool                        parseListenEndpoint(const std::string& listenValue, ListenEndpoint& endpoint);

    // Create a non-blocking TCP listening socket bound to the endpoint, or -1 on failure.
    int                         createListenSocket(const ListenEndpoint& endpoint);

    // Enable non-blocking mode and FD_CLOEXEC on a socket descriptor.
    bool                        setSocketNonBlocking(int fd);

    // Send the full buffer using non-blocking send when POLLOUT is active.
    ssize_t                     sendSocketData(int fd, const char* data, size_t length, size_t offset);

    // Return a short description for the last socket setup failure (bind/listen).
    inline std::string          describeLastSocketError() { return (std::strerror(errno)); }

}

#endif
