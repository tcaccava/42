#include "poll/IoPollHub.hpp"
#include "Config.hpp"
#include "io/SocketIo.hpp"
#include <ctime>
#include <exception>
#include <sys/socket.h>
#include <unistd.h>

namespace webserver {

    IoPollHub::IoPollHub()
        : pollEntries_(),
          pollSlots_(),
          listenSlots_(),
          clientConnections_() {
    }

    IoPollHub::~IoPollHub() {
        for (size_t i = 0; i < clientConnections_.size(); i++) {
            delete clientConnections_[i];
            clientConnections_[i] = NULL;
        }
        clientConnections_.clear();

        for (size_t i = 0; i < listenSlots_.size(); i++) {
            if (listenSlots_[i].fd >= 0)
                ::close(listenSlots_[i].fd);
        }
        listenSlots_.clear();
        pollEntries_.clear();
        pollSlots_.clear();
    }

    void IoPollHub::registerPollFd(int fd, const PollSlot& slot) {
        pollfd entry;
        entry.fd = fd;
        entry.events = (slot.client != NULL) ? slot.client->getPollEvents() : POLLIN;
        entry.revents = 0;
        pollEntries_.push_back(entry);
        pollSlots_.push_back(slot);
    }

    void IoPollHub::unregisterPollFd(int fd) {
        for (size_t i = 0; i < pollEntries_.size(); i++) {
            if (pollEntries_[i].fd != fd)
                continue;

            pollEntries_.erase(pollEntries_.begin() + static_cast<std::vector<pollfd>::difference_type>(i));
            pollSlots_.erase(pollSlots_.begin() + static_cast<std::vector<PollSlot>::difference_type>(i));
            return;
        }
    }

    void IoPollHub::updatePollEvents(ClientConnection& connection) {
        for (size_t i = 0; i < pollEntries_.size(); i++) {
            if (pollEntries_[i].fd != connection.getFd())
                continue;

            pollEntries_[i].events = connection.getPollEvents();
            return;
        }
    }

    void IoPollHub::addListenSocket(int fd, size_t serverIndex) {
        ListenSlot slot;
        slot.fd = fd;
        slot.serverIndex = serverIndex;
        listenSlots_.push_back(slot);

        PollSlot pollSlot;
        pollSlot.isListen = true;
        pollSlot.listenIndex = listenSlots_.size() - 1;
        pollSlot.client = NULL;
        registerPollFd(fd, pollSlot);
    }

    void IoPollHub::acceptClient(int listenFd, size_t serverIndex, const ServerData& server) {
        const int clientFd = ::accept(listenFd, NULL, NULL);
        if (clientFd < 0)
            return;

        if (!setSocketNonBlocking(clientFd)) {
            ::close(clientFd);
            return;
        }

        ClientConnection* connection = new ClientConnection(clientFd, serverIndex, server);
        clientConnections_.push_back(connection);

        PollSlot pollSlot;
        pollSlot.isListen = false;
        pollSlot.listenIndex = 0;
        pollSlot.client = connection;
        registerPollFd(clientFd, pollSlot);
    }

    void IoPollHub::run(const RequestHandler& handler) {
        static const double CLIENT_TIMEOUT_SECONDS = 30.0;

        while (!pollEntries_.empty()) {
            const int readyCount = ::poll(&pollEntries_[0], pollEntries_.size(), 5000);
            if (readyCount < 0)
                continue;

            if (readyCount == 0) {
                const time_t now = time(NULL);
                for (size_t i = 0; i < pollSlots_.size(); ) {
                    if (!pollSlots_[i].isListen && pollSlots_[i].client != NULL) {
                        ClientConnection* conn = pollSlots_[i].client;
                        if (difftime(now, conn->getLastActivity()) > CLIENT_TIMEOUT_SECONDS) {
                            const int fd = pollEntries_[i].fd;
                            conn->close();
                            unregisterPollFd(fd);
                            continue;
                        }
                    }
                    i++;
                }
                continue;
            }

            for (size_t i = 0; i < pollEntries_.size(); ) {
                pollfd& entry = pollEntries_[i];
                PollSlot& slot = pollSlots_[i];
                const short revents = entry.revents;
                entry.revents = 0;

                if (revents == 0) {
                    i++;
                    continue;
                }

                if ((revents & (POLLERR | POLLHUP | POLLNVAL)) != 0) {
                    if (!slot.isListen && slot.client != NULL) {
                        slot.client->close();
                        unregisterPollFd(entry.fd);
                    }
                    continue;
                }

                if (slot.isListen) {
                    if ((revents & POLLIN) != 0) {
                        const ListenSlot& listenSlot = listenSlots_[slot.listenIndex];
                        acceptClient(listenSlot.fd, listenSlot.serverIndex,
                            Config::get().servers[listenSlot.serverIndex]);
                    }
                    i++;
                    continue;
                }

                ClientConnection* connection = slot.client;
                if (connection == NULL) {
                    unregisterPollFd(entry.fd);
                    continue;
                }

                bool keepOpen = true;

                if ((revents & POLLIN) != 0) {
                    try {
                        keepOpen = connection->onPollIn(handler);
                    } catch (const std::exception&) {
                        keepOpen = false;
                    }
                }

                if (keepOpen && connection->getIoState() == CLIENT_IO_WRITING)
                    updatePollEvents(*connection);

                if (keepOpen && (revents & POLLOUT) != 0) {
                    try {
                        keepOpen = connection->onPollOut();
                    } catch (const std::exception&) {
                        keepOpen = false;
                    }
                }

                if (!keepOpen) {
                    connection->close();
                    unregisterPollFd(entry.fd);
                    continue;
                }

                i++;
            }
        }
    }

}
