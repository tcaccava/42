#include "connection/ClientConnection.hpp"
#include "handler/RequestHandler.hpp"
#include "io/SocketIo.hpp"
#include <ctime>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace webserver {

    static const size_t kRecvBufferSize = 65536;

    ClientConnection::ClientConnection(int fd, size_t serverIndex, const ServerData& server)
        : fd_(fd),
          serverIndex_(serverIndex),
          server_(server),
          parser_(server.client_max_body_size),
          ioState_(CLIENT_IO_READING),
          outboundWire_(),
          outboundOffset_(0),
          lastActivity_(time(NULL)) {
    }

    ClientConnection::~ClientConnection() {
        close();
    }

    short ClientConnection::getPollEvents() const {
        if (ioState_ == CLIENT_IO_WRITING)
            return (POLLOUT);
        if (ioState_ == CLIENT_IO_READING)
            return (POLLIN);
        return (0);
    }

    void ClientConnection::beginResponse(HttpResponse& response) {
        response.setHeader("Connection", "close");
        outboundWire_ = response.toWireFormat();
        outboundOffset_ = 0;
        ioState_ = CLIENT_IO_WRITING;
    }

    bool ClientConnection::onPollIn(const RequestHandler& handler) {
        char buffer[kRecvBufferSize];

        const ssize_t received = ::recv(fd_, buffer, sizeof(buffer), 0);
        if (received < 0)
            return (false);
        if (received == 0)
            return (false);
        lastActivity_ = time(NULL);

        const HttpParseStatus status = parser_.appendReceivedOctets(buffer, static_cast<size_t>(received));

        if (status == HTTP_PARSE_INCOMPLETE)
            return (true);

        if (status == HTTP_PARSE_ERROR) {
            HttpResponse response;
            if (parser_.isRequestBodyTooLargeError())
                handler.respondConfiguredError(response, server_, 413);
            else {
                response.setStatus(400);
                response.setHeader("Content-Type", "text/plain; charset=utf-8");
                response.setBody("Bad Request: " + parser_.getParseErrorMessage());
            }
            beginResponse(response);
            return (true);
        }

        HttpResponse response;
        handler.handle(parser_.getParsedRequest(), server_, response);
        beginResponse(response);
        return (true);
    }

    bool ClientConnection::onPollOut() {
        const ssize_t sent = sendSocketData(fd_, outboundWire_.c_str(), outboundWire_.size(), outboundOffset_);
        if (sent < 0)
            return (false);
        if (sent == 0)
            return (false);

        outboundOffset_ += static_cast<size_t>(sent);
        if (outboundOffset_ >= outboundWire_.size())
            return (false);

        return (true);
    }

    void ClientConnection::close() {
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
        ioState_ = CLIENT_IO_CLOSED;
    }

}
