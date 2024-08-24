#include "Logger.h"
#include "Config.h"
#include "General.h"
#include <iostream>

Logger::Logger() {
    Config& config = Config::getInstance();
    const std::string& logFileName = config.get("logFileName");
    if (logFileName == EMPTY_STRING) 
        logFile.open(DEFAULT_LOG_FILE_NAME, std::ios::out);
    else
        logFile.open(logFileName, std::ios::out);

    if (!logFile.is_open()) {
        std::cerr << "Unable to open log file." << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::logToStdout(const std::string& message) {
    std::cout << message << std::endl;
}

void Logger::logToStderr(const std::string& message) {
    std::cerr << message << std::endl;
}

void Logger::logToFile(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}

void Logger::logInfo(const std::string& message) {
    std::string logMessage = "[\033[32mINFO\033[0m] " + message;
    logToStdout(logMessage);
    logToFile(logMessage);
}

void Logger::logError(const std::string& message) {
    std::string logMessage = "[\033[31mERROR\033[0m] " + message;
    logToStderr(logMessage);
    logToFile(logMessage);
}
