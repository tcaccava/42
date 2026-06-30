#include "../includes/Logger.hpp"
#include <ctime>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

namespace webserver {
    static std::ofstream& getLogFile() {
        static std::ofstream logFile;
        return (logFile);
    }

    // The logs/ directory must exist before writing.
    // Output path: logs/webserver_log.txt (or ../logs/ when run from a subfolder).
    static bool initLogging() {
        static bool initialized = false;

        if (initialized)
            return (getLogFile().is_open());
        initialized = true;

        std::ofstream& logFile = getLogFile();
        logFile.open("logs/webserver_log.txt", std::ios::out | std::ios::trunc);
        if (logFile.is_open())
            return (true);

        logFile.open("../logs/webserver_log.txt", std::ios::out | std::ios::trunc);
        if (logFile.is_open())
            return (true);

        std::cerr << "[WARNING] logs/ directory not found. Please create a logs/ directory in the root to enable WebServ logging." << std::endl;
        return (false);
    }

    LogStream::LogStream(LogLevel level, const std::string funcName) : level_(level), func_(funcName) {
        if (!initLogging()) {
            level_ = NOT_SET;
            return;
        }

        char buffer[20];
        std::time_t now = std::time(NULL);
        std::tm *localTime = std::localtime(&now);

        if (localTime && std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime))
            timestamp_ = buffer;
        else
            timestamp_ = "0000-00-00 00:00:00";
    }

    LogStream& LogStream::operator<<(std::ostream& (*manip)(std::ostream&)) {
        stream_ << manip;
        return (*this);
    }

    LogStream::~LogStream() {
        if (level_ == NOT_SET)
            return;

        std::ofstream& logFile = getLogFile();
        if (!logFile.is_open())
            return;

        logFile << "[" << timestamp_ << "]";
        logFile << " [";
        switch (level_) {
            case SUCCESS:
                logFile << std::left << std::setw(7) << "SUCCESS";
                break;
            case WARNING:
                logFile << std::left << std::setw(7) << "WARNING";
                break;
            case ERROR:
                logFile << std::left << std::setw(7) << "ERROR";
                break;
            case DEBUG:
                logFile << std::left << std::setw(7) << "DEBUG";
                break;
            default:
                break;
        }
        logFile << "]";
        logFile << " << " << std::left << std::setw(25) << ("[" + func_ + "]");
        logFile << " << ";
        std::string msg = stream_.str();
        msg.erase(0, msg.find_first_not_of(" \n\t"));
        msg.erase(msg.find_last_not_of(" \n\t") + 1);
        logFile << msg;
        logFile << std::endl;
    }

    void LogStream::closeLogFile() {
        std::ofstream& logFile = getLogFile();
        if (logFile.is_open())
            logFile.close();
    }
}
