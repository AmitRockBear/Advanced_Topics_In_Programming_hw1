#include "Utils.h"
#include "Logger.h"
#include <thread>
#include <sstream>

void clear() {
  #if defined _WIN32
      system("cls");
  #elif defined (_LINUX) || defined(gnu_linux) || defined(linux_)
      system("clear");
  #elif defined (_APPLE_)
      system("clear");
  #endif
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
