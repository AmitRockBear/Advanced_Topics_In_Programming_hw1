#ifndef HOUSE_WRAPPER_H
#define HOUSE_WRAPPER_H
#include <string>
#include <vector>

class HouseWrapper {
public:
    HouseWrapper() = default;
    const std::string& getHouseFileName() const;
    bool getIsValid() const;

    void setHouseFileName(const std::string& houseFileName);
    void setIsValid(bool isValid);

    HouseWrapper(const HouseWrapper&) = delete;
    HouseWrapper& operator=(const HouseWrapper&) = delete;
    HouseWrapper(HouseWrapper&&) = delete;
    HouseWrapper& operator=(HouseWrapper&&) = delete;

    static void createHouseWrappers(const std::vector<std::string>& houseFileNames, std::vector<HouseWrapper>& houseWrappers);
private:
    std::string houseFileName;
    bool isValid = true;
};

#endif
