#include "minidb.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

// Puntatore globale per il signal handler
static Server *g_server_for_signal = 0;

// ---------------- MiniDB ----------------
MiniDB::MiniDB(const std::string &filepath) : filepath_(filepath) {}

// Carica il database dal file
void MiniDB::load()
{
    std::ifstream in(filepath_.c_str());
    if (!in.is_open())
        return;

    std::string key, value;
    while (in >> key >> value)
    {
        db_[key] = value;
    }
    in.close();
}

// Salva il database nel file
void MiniDB::save()
{
    std::ofstream out(filepath_.c_str());
    if (!out.is_open())
    {
        std::cerr << "Invalid file path\n";
        return;
    }
    std::map<std::string, std::string>::const_iterator it;
    for (it = db_.begin(); it != db_.end(); ++it)
    {
        out << it->first << " " << it->second << "\n";
    }
    out.close();
}

// Gestisce i comandi POST, GET, DELETE
std::string MiniDB::handleCommand(const std::string &cmd)
{
    std::istringstream ss(cmd);
    std::string op, key, value;
    ss >> op;

    if (op == "POST")
    {
        if (!(ss >> key >> value))
            return "1\n"; // chiave o valore mancanti
        db_[key] = value;
        return "0\n";
    }
    else if (op == "GET")
    {
        if (!(ss >> key))
            return "1\n"; // chiave non passata
        std::map<std::string, std::string>::iterator it = db_.find(key);
        if (it == db_.end())
            return "1\n"; // chiave non esiste
        return "0 " + it->second + "\n";
    }
    else if (op == "DELETE")
    {
        if (!(ss >> key))
            return "1\n"; // chiave non passata
        if (db_.erase(key))
            return "0\n"; // cancellata correttamente
        return "1\n";     // chiave non esiste
    }

    return "2\n"; // comando sconosciuto
}

// ---------------- Server ----------------
Server::Server(int port, MiniDB &db) : port_(port), db_(db)
{
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0)
        throw std::runtime_error("Socket creation failed");

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(server_fd_, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Socket bind failed");

    if (listen(server_fd_, 10) < 0)
        throw std::runtime_error("Socket listen failed");

    g_server_for_signal = this;
}

Server::~Server()
{
    if (server_fd_ >= 0)
        close(server_fd_);
}

// Signal handler Ctrl+C
static void sigint_handler(int)
{
    if (g_server_for_signal)
        g_server_for_signal->db_.save();
    exit(0);
}

// Legge una riga dalla socket (fino a '\n')
bool readLine(int fd, std::string &out)
{
    out.clear();
    char c;
    while (1)
    {
        ssize_t r = recv(fd, &c, 1, 0);
        if (r <= 0)
            return false;
        if (c == '\n')
            return true;
        out.push_back(c);
    }
}

void Server::run()
{
    signal(SIGINT, sigint_handler);

    fd_set active, readfds;
    FD_ZERO(&active);
    FD_SET(server_fd_, &active);
    int maxfd = server_fd_;
    std::cout << "Server is now ready to accept connections" << std::endl;

    while (1)
    {
        readfds = active;
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0)
            continue; // throw std::runtime_error("error in select");

        for (int fd = 0; fd <= maxfd; ++fd)
        {
            if (!FD_ISSET(fd, &readfds))
                continue;

            if (fd == server_fd_)
            {
                int client = accept(server_fd_, NULL, NULL);
                // if (client < 0)
                //     throw std::runtime_error("Failed to accept connection");
                if (client >= 0)
                {
                    FD_SET(client, &active);
                    if (client > maxfd)
                        maxfd = client;
                }
            }
            else
            {
                std::string line;
                if (!readLine(fd, line))
                {
                    close(fd);
                    FD_CLR(fd, &active);
                }
                else
                {
                    std::string res = db_.handleCommand(line);
                    write(fd, res.c_str(), res.size());
                }
            }
        }
    }
}

// ---------------- Main ----------------
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Wrong number of argument\n";
        return 1;
    }
    try
    {
        int port = atoi(argv[1]);
        std::string filepath = argv[2];

        MiniDB db(filepath);
        db.load();

        Server server(port, db);
        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
