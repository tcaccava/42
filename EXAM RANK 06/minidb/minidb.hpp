#ifndef MINIDB_HPP
#define MINIDB_HPP

#include <string>
#include <map>

class MiniDB {
private:
    std::map<std::string, std::string> db_;
    std::string filepath_;
public:
    MiniDB(const std::string &filepath);

    void load();
    void save();

    std::string handleCommand(const std::string &cmd);
};

class Server {
private:
    int port_;
    int server_fd_;
    
public:
    Server(int port, MiniDB &db);
    ~Server();
    MiniDB &db_;
    void run();
};

#endif
