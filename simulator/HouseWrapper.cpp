#include "HouseWrapper.h"

const std::string& HouseWrapper::getHouseFileName() const { return houseFileName; }

bool HouseWrapper::getIsValid() const { return isValid; }

void HouseWrapper::setHouseFileName(const std::string& houseFileName) { this->houseFileName = houseFileName; }
void HouseWrapper::setIsValid(bool isValid) { this->isValid = isValid; }

void HouseWrapper::createHouseWrappers(const std::vector<std::string>& houseFileNames, std::vector<HouseWrapper>& houseWrappers) {
    for (std::size_t i = 0; i < houseFileNames.size(); ++i) {
        houseWrappers[i].setHouseFileName(houseFileNames[i]);
    }
}