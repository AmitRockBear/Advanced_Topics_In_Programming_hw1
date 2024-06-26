#ifndef UTILS_H
#define UTILS_H

void clear();
const std::string trim(const std::string& str);
bool isNextCharacterSpaceOrEndOfLine(std::istringstream& iss);
void handleException(const std::exception& e);

#endif