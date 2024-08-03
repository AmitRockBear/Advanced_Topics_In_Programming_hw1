#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>

class Logger {
public:
    static Logger& getInstance();
    void logInfo(const std::string &message);
    void logError(const std::string& message);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;
private:
    Logger();
    ~Logger();
    void logToFile(const std::string& message);
    void logToStdout(const std::string &message);
    void logToStderr(const std::string &message);

    std::ofstream logFile;
};

#endif
