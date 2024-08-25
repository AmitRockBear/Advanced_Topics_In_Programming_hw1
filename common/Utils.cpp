#include "Utils.h"
#include "Logger.h"
#include "General.h"
#include <thread>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void clear() {
  #if defined _WIN32
      system("cls");
  #elif defined (_LINUX) || defined(gnu_linux) || defined(linux_)
      system("clear");
  #elif defined (_APPLE_)
      system("clear");
  #endif
}

const std::string getFileBaseName(const std::string& path) {
    if (path.empty()) return ""; // Handle empty path

    size_t sep_pos = path.find_last_of("/\\");
    if (sep_pos == std::string::npos) return path; // No separator, return as is

    std::string filename = path.substr(sep_pos + 1);
    size_t last_dot = filename.find_last_of('.');
    if (last_dot != std::string::npos) {
        return filename.substr(0, last_dot);
    }
    return filename;
}


const std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == std::string::npos || last == std::string::npos)
        return "";

    return str.substr(first, last - first + 1);
}

bool isNextCharacterSpaceOrEndOfLine(std::istringstream& iss) {
    int nextCharacter = iss.peek();

    if (nextCharacter != EOF && !std::isspace(nextCharacter)) {
        return false;
    }
    return true;
}

void handleMainThreadException(const std::exception& e) {
    Logger::getInstance().logError(std::string(e.what()));
}

bool extractKeyValue(std::string& line, std::string& key, std::string& value) {
    std::istringstream lineStream(line);
    if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
        key = trim(key);
        value = trim(value);

        if (key.empty() || value.empty()) {
            return false;
        }
        return true;
    }
    return false;
}

bool appendToErrorFile(const std::string& fileName, const std::string& message) {
    const std::string errorFileName = fileName + ".error";
    std::ofstream errorFile(errorFileName, std::ios::app);
    if (!errorFile.is_open()) {
        return false;
    }
    errorFile << message << std::endl;
    errorFile.close();
    return true;
}

std::tuple<std::ptrdiff_t, bool> parseArguments(int argc, char* argv[], std::string& housePath, std::string& algoPath) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Parsing arguments");

    std::ptrdiff_t numThreads = DEFAULT_NUM_THREADS_VALUE;
    bool isSummaryOnly = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind(DEFAULT_HOUSE_ARG, 0) == 0) {
            housePath = arg.substr(DEFAULT_HOUSE_ARG.length());
        } else if (arg.rfind(DEFAULT_ALGORITHM_ARG, 0) == 0) {
            algoPath = arg.substr(DEFAULT_ALGORITHM_ARG.length());
        } else if (arg.rfind(DEFAULT_NUM_THREADS_ARG, 0) == 0) {
            numThreads = std::stoul(arg.substr(DEFAULT_NUM_THREADS_ARG.length()));
        } else if (arg.rfind(DEFAULT_SUMMARY_ARG, 0) == 0) {
            isSummaryOnly = true;
        }
    }

    logger.logInfo("Arguments parsed successfully");

    return std::make_tuple(numThreads, isSummaryOnly);
}

void findHouseFiles(const std::string& housePath, std::vector<std::string>& houseFilePaths) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Finding house files");

    for (const auto& entry : fs::directory_iterator(housePath)) {
        if (entry.is_regular_file() && entry.path().extension() == DEFAULT_HOUSE_FILE_EXTENSION) {
            houseFilePaths.push_back(entry.path().string());
        }
    }
    
    logger.logInfo("All house files found successfully");
}

void findAlgoFiles(const std::string& algoPath, std::vector<std::string>& algoFilePaths) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Finding algorithm files");

    for (const auto& entry : fs::directory_iterator(algoPath)) {
        if (entry.is_regular_file() && entry.path().extension() == DEFAULT_ALGORITHM_FILE_EXTENSION) {
            algoFilePaths.push_back(entry.path().string());
        }
    }

    logger.logInfo("All algorithm files found successfully");
}