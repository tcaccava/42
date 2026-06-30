#ifndef IO_POLL_HUB_HPP
#define IO_POLL_HUB_HPP

#include "connection/ClientConnection.hpp"
#include <poll.h>
#include <vector>

namespace webserver {

    class RequestHandler;

    struct ListenSlot {
        int         fd;
        size_t      serverIndex;
    };

    struct PollSlot {
        bool                isListen;
        size_t              listenIndex;
        ClientConnection*   client;
    };

    class IoPollHub {

        private:
            std::vector<pollfd>             pollEntries_;
            std::vector<PollSlot>             pollSlots_;
            std::vector<ListenSlot>           listenSlots_;
            std::vector<ClientConnection*>    clientConnections_;

            void                registerPollFd(int fd, const PollSlot& slot);
            void                unregisterPollFd(int fd);
            void                updatePollEvents(ClientConnection& connection);

            IoPollHub(const IoPollHub& other);
            IoPollHub& operator=(const IoPollHub& other);

        public:
            IoPollHub();

            // Register a listening socket for a server block index from the configuration.
            void                addListenSocket(int fd, size_t serverIndex);

            // Accept a new client on a listening socket and register it for POLLIN.
            void                acceptClient(int listenFd, size_t serverIndex, const ServerData& server);

            // Run the single poll loop required by the subject.
            void                run(const RequestHandler& handler);

            ~IoPollHub();
    };

}

#endif
