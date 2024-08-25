#include "ThreadController.h"

std::string ThreadController::getHouseFileBaseName() const {
    return houseFileBaseName;
}

std::string ThreadController::getAlgorithmFileBaseName() const {
    return algorithmFileBaseName;
}

AbstractAlgorithm* ThreadController::getCreatedAlgorithmPointer() const {
    if (createdAlgorithmPointer) {
        return createdAlgorithmPointer.get();
    }
    return nullptr;
}

std::size_t ThreadController::getScore() const {
    return score;
}

std::string ThreadController::getHouseError() const {
    return houseError;
}

std::string ThreadController::getAlgorithmError() const {
    return algorithmError;
}

bool ThreadController::isJoined() const {
    return joined;
}

void ThreadController::setHouseFileBaseName(const std::string& houseFileBaseName) {
    this->houseFileBaseName = houseFileBaseName;
}

void ThreadController::setAlgorithmFileBaseName(const std::string& algorithmFileBaseName) {
    this->algorithmFileBaseName = algorithmFileBaseName;
}

void ThreadController::setCreatedAlgorithmPointer(std::unique_ptr<AbstractAlgorithm> createdAlgorithmPointer) {
    this->createdAlgorithmPointer = std::move(createdAlgorithmPointer);
}

void ThreadController::setScore(std::size_t score) {
    this->score = score;
}

void ThreadController::setHouseError(const std::string& error) {
    this->houseError = error;
}

void ThreadController::setAlgorithmError(const std::string& error) {
    this->algorithmError = error;
}

void ThreadController::setJoined(bool joined) {
    this->joined = joined;
}

void ThreadController::resetCreatedAlgorithmPointer() {
    if (createdAlgorithmPointer)
        createdAlgorithmPointer.reset();
}