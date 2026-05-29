#include "Socket.hpp"

Socket::Socket() : fd(-1) {}

Socket::Socket(const Socket &toCopy) {
    // Transfer ownership: il socket originale rilascia la proprietà
    fd = toCopy.fd;
    const_cast<Socket&>(toCopy).fd = -1;  // Impedisce al destructor dell'originale di chiudere il socket
}

Socket::~Socket() {
    if (fd != -1) {
        closeSocket();
    }
}

Socket& Socket::operator=(const Socket &other) {
    if (this != &other) {
        // Chiudi il socket corrente se esiste
        if (fd != -1) {
            closeSocket();
        }
        // Transfer ownership: l'altro socket rilascia la proprietà
        fd = other.fd;
        const_cast<Socket&>(other).fd = -1;
    }
    return *this;
}

void Socket::create() {
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw SocketException("Socket creation failed: " + std::string(strerror(errno)));
    std::cout << "Socket created successfully." << std::endl;
    
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw SocketException("setsockopt failed");
}

void Socket::bind(int port) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (::bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        throw SocketException("Socket bound failed: " + std::string(strerror(errno)));
    std::cout << "Socket bound to port " << port << "." << std::endl;
    std::cout << "http://localhost:" << port << std::endl;
}

void Socket::listen() {
    if (::listen(fd, SOMAXCONN) == -1)
        throw SocketException("Socket listening failed: " + std::string(strerror(errno)));
    std::cout << "Server is listening for connections." << std::endl;
}

int Socket::accept() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = ::accept(fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        throw SocketException("Error accepting connection");
    }
    return client_fd;
}

void Socket::closeSocket() {
    if (fd != -1) {
        close(fd);
        std::cout << "Socket closed." << std::endl;
    }
}

int Socket::getFd() const {
    return fd;
}

void Socket::setFd(int fd) {
    this->fd = fd;
    return ;
}

/**
 * @brief Output Socket's file descriptor for debugging
 *
 * Example: "Socket[fd=4]"
 */
std::ostream& operator<<(std::ostream &os, const Socket &sock) {
    os << "Socket[fd=" << sock.getFd() << "]";
    return os;
}
