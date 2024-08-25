#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <string>
#include <memory>
#include "../common/abstract_algorithm.h"

class ThreadController {
public:
    ThreadController() = default;

    std::string getHouseFileBaseName() const;
    std::string getAlgorithmFileBaseName() const;
    AbstractAlgorithm* getCreatedAlgorithmPointer() const;
    std::size_t getScore() const;
    std::string getHouseError() const;
    std::string getAlgorithmError() const;
    bool isJoined() const;

    void setHouseFileBaseName(const std::string& houseFileBaseName);
    void setAlgorithmFileBaseName(const std::string& algorithmFileBaseName);
    void setCreatedAlgorithmPointer(std::unique_ptr<AbstractAlgorithm> createdAlgorithmPointer);
    void setScore(std::size_t score);
    void setHouseError(const std::string& error);
    void setAlgorithmError(const std::string& error);
    void setJoined(bool joined);
    void resetCreatedAlgorithmPointer();

private:
    std::string houseFileBaseName;
    std::string algorithmFileBaseName;
    std::unique_ptr<AbstractAlgorithm> createdAlgorithmPointer;
    std::size_t score;
    std::string houseError;
    std::string algorithmError;
    bool joined = false;
};

#endif // THREADCONTROLLER_H
