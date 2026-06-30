#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include "Http.hpp"
#include "Parsing.hpp"
#include <ctime>
#include <string>

namespace webserver {

    class RequestHandler;

    enum ClientIoState {
        CLIENT_IO_READING,
        CLIENT_IO_WRITING,
        CLIENT_IO_CLOSED
    };

    class ClientConnection {

        private:
            int                 fd_;
            size_t              serverIndex_;
            const ServerData&   server_;
            HttpParser          parser_;
            ClientIoState       ioState_;
            std::string         outboundWire_;
            size_t              outboundOffset_;
            time_t              lastActivity_;

            // Build the wire response after a complete request has been parsed.
            void                beginResponse(HttpResponse& response);

            ClientConnection(const ClientConnection& other);
            ClientConnection& operator=(const ClientConnection& other);

        public:
            // Attach a new accepted client socket to a server configuration block.
            ClientConnection(int fd, size_t serverIndex, const ServerData& server);

            inline int          getFd() const { return (fd_); }
            inline ClientIoState getIoState() const { return (ioState_); }
            inline time_t       getLastActivity() const { return (lastActivity_); }
            short               getPollEvents() const;

            // Consume readable data from the socket; return false when the connection must close.
            bool                onPollIn(const RequestHandler& handler);

            // Flush the outbound response buffer; return false when the connection must close.
            bool                onPollOut();

            // Mark the connection closed and release the socket descriptor.
            void                close();

            ~ClientConnection();
    };

}

#endif
