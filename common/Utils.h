#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <sstream>

void clear();
const std::string trim(const std::string& str);
bool isNextCharacterSpaceOrEndOfLine(std::istringstream& iss);
void handleMainThreadException(const std::exception& e);
const std::string getFileBaseName(const std::string& path);
bool extractKeyValue(std::string& line, std::string& key, std::string& value);
bool appendToErrorFile(const std::string& fileName, const std::string& message);

#endif