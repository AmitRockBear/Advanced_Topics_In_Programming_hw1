#include "Utils.h"
#include "Logger.h"
#include <thread>

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
    std::string filename = path.substr(path.find_last_of("/\\") + 1);
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

void handleException(const std::exception& e) {
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