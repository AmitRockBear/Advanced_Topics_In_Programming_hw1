#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <sstream>
#include <vector>

void clear();
const std::string trim(const std::string& str);
bool isNextCharacterSpaceOrEndOfLine(std::istringstream& iss);
void handleMainThreadException(const std::exception& e);
const std::string getFileBaseName(const std::string& path);
bool extractKeyValue(std::string& line, std::string& key, std::string& value);
bool appendToErrorFile(const std::string& fileName, const std::string& message);
std::tuple<std::ptrdiff_t, bool> parseArguments(int argc, char* argv[], std::string& housePath, std::string& algoPath);
void findHouseFiles(const std::string& housePath, std::vector<std::string>& houseFilePaths);
void findAlgoFiles(const std::string& algoPath, std::vector<std::string>& algoFilePaths);

#endif