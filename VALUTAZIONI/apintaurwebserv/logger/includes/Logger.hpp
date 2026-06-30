#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <sstream>
#include <string>

namespace webserver {

    inline std::string to_logNumber(const size_t n) {
        if (n == std::string::npos)
            return ("undefined");

        std::ostringstream out;
        out << n;
        return (out.str());
    }

    #define WS_FUNCTION() (std::string(__FILE__) + ":" + webserver::to_logNumber(__LINE__) + "][" + __func__)

    enum LogLevel {
        SUCCESS,
        WARNING,
        DEBUG,
        ERROR,
        NOT_SET
    };

    class LogStream {

        private:
            LogLevel            level_;
            std::string         func_;
            std::string         timestamp_;
            std::ostringstream  stream_;

            LogStream(const LogStream& other);
            LogStream& operator=(const LogStream& other);

        public:
            // Requires an existing logs/ directory before the first log write.
            LogStream(LogLevel level, const std::string funcName);

            template<typename T>
            LogStream& operator<<(const T& msg) {
                stream_ << msg;
                return (*this);
            }

            LogStream& operator<<(std::ostream& (*manip)(std::ostream&));

            static void closeLogFile();

            ~LogStream();
    };

}

#define Logger(level) webserver::LogStream((level), WS_FUNCTION())

#endif
