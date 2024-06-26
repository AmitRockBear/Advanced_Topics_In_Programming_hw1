#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <sstream>

void clear();
const std::string trim(const std::string& str);
bool isNextCharacterSpaceOrEndOfLine(std::istringstream& iss);
void handleException(const std::exception& e);

#endif