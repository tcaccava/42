#include "Client.hpp"
#include "Config.hpp"
#include "io/SocketIo.hpp"
#include "poll/IoPollHub.hpp"
#include "handler/RequestHandler.hpp"
#include "Logger.hpp"

namespace webserver {

    void runWebServer(const WebserverConfig& config) {
        if (config.servers.empty()) {
            Logger(ERROR) << "No server blocks in configuration" << std::endl;
            return;
        }

        IoPollHub pollHub;
        size_t listenCount = 0;

        for (size_t serverIndex = 0; serverIndex < config.servers.size(); serverIndex++) {
            const ServerData& server = config.servers[serverIndex];

            for (size_t i = 0; i < server.listen.size(); i++) {
                ListenEndpoint endpoint;
                if (!parseListenEndpoint(server.listen[i], endpoint)) {
                    Logger(ERROR) << "Invalid listen directive: " << server.listen[i] << std::endl;
                    continue;
                }

                const int listenFd = createListenSocket(endpoint);
                if (listenFd < 0) {
                    Logger(ERROR) << "Failed to listen on " << server.listen[i] << " (" << describeLastSocketError() << ")" << std::endl;
                    continue;
                }

                pollHub.addListenSocket(listenFd, serverIndex);
                listenCount++;
                Logger(SUCCESS) << "Listening on " << server.listen[i] << std::endl;
            }
        }

        if (listenCount == 0) {
            Logger(ERROR) << "No listening sockets created" << std::endl;
            return;
        }

        RequestHandler handler;
        pollHub.run(handler);
    }

}
