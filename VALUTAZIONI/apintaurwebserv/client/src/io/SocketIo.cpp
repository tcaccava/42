#include "io/SocketIo.hpp"
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace webserver {

    static bool parsePortToken(const std::string& token, int& port) {
        if (token.empty())
            return (false);

        char* end = NULL;
        const long value = std::strtol(token.c_str(), &end, 10);
        if (end == NULL || *end != '\0' || value < 1 || value > 65535)
            return (false);

        port = static_cast<int>(value);
        return (true);
    }

    bool parseListenEndpoint(const std::string& listenValue, ListenEndpoint& endpoint) {
        endpoint.host.clear();
        endpoint.port = 0;

        const size_t colonPos = listenValue.rfind(':');
        if (colonPos == std::string::npos)
            return (parsePortToken(listenValue, endpoint.port));

        if (colonPos == 0 || colonPos + 1 >= listenValue.size())
            return (false);

        endpoint.host = listenValue.substr(0, colonPos);
        return (parsePortToken(listenValue.substr(colonPos + 1), endpoint.port));
    }

    bool setSocketNonBlocking(int fd) {
        const int flags = ::fcntl(fd, F_GETFL, 0);
        if (flags < 0)
            return (false);

        if (::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
            return (false);

        if (::fcntl(fd, F_SETFD, FD_CLOEXEC) < 0)
            return (false);

        return (true);
    }

    int createListenSocket(const ListenEndpoint& endpoint) {
        const int listenFd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (listenFd < 0)
            return (-1);

        int reuse = 1;
        if (::setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
            ::close(listenFd);
            return (-1);
        }

        struct sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(static_cast<uint16_t>(endpoint.port));

        if (endpoint.host.empty()) {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else {
            addr.sin_addr.s_addr = ::inet_addr(endpoint.host.c_str());
            if (addr.sin_addr.s_addr == INADDR_NONE) {
                ::close(listenFd);
                return (-1);
            }
        }

        if (::bind(listenFd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) != 0) {
            ::close(listenFd);
            return (-1);
        }

        if (::listen(listenFd, 128) != 0) {
            ::close(listenFd);
            return (-1);
        }

        if (!setSocketNonBlocking(listenFd)) {
            ::close(listenFd);
            return (-1);
        }

        return (listenFd);
    }

    ssize_t sendSocketData(int fd, const char* data, size_t length, size_t offset) {
        if (data == NULL || offset >= length)
            return (0);

        return (::send(fd, data + offset, length - offset, 0));
    }

}
