#include "AlgorithmWrapper.h"
#include "../common/Utils.h"
#include "../common/Logger.h"
#include "../common/General.h"

AlgorithmWrapper::AlgorithmWrapper(const std::string& algorithmFileName)
    : algorithmFileName(algorithmFileName), dlOpenPointer(nullptr), index(0), isValid(true) {}

AlgorithmWrapper::~AlgorithmWrapper() {
    if (dlOpenPointer) {
        dlclose(dlOpenPointer);
    }
}

bool AlgorithmWrapper::registerAlgorithm() {
    Logger& logger = Logger::getInstance();
    const std::string& algoFilePath = algorithmFileName;
    dlOpenPointer = dlopen(algoFilePath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!dlOpenPointer) {
        const std::string errorMessage = "Failed to dlopen algorithm: " + getFileBaseName(algoFilePath) + " " + std::string(dlerror());
        logger.logError(errorMessage);
        appendToErrorFile(algoFilePath, errorMessage);
        isValid = false;
        return false;
    }
    return true;
}

bool AlgorithmWrapper::hasAlgorithmRegisteredSuccessfully(std::size_t countBeforeRegistering) {
    Logger& logger = Logger::getInstance();
    if (countBeforeRegistering == AlgorithmRegistrar::getAlgorithmRegistrar().count()) {
        isValid = false;
        const std::string& algoFilePath = algorithmFileName;
        const std::string errorMessage = "Failed to register algorithm: " + getFileBaseName(algoFilePath) + " number of algorithms in the registrar is not increased after opening the .so";
        logger.logError(errorMessage);
        appendToErrorFile(algoFilePath, errorMessage);
        return false;
    }
    return true;
}


void AlgorithmWrapper::createAlgorithmWrappers(const std::vector<std::string>& algoFilePaths, std::vector<AlgorithmWrapper>& algorithmWrappers) {
    Logger& logger = Logger::getInstance();    
    std::size_t i;
    std::size_t countBeforeRegistering;

    logger.logInfo("Registering algorithms");

    for (i = 0; i < algoFilePaths.size(); i++) {
        countBeforeRegistering = AlgorithmRegistrar::getAlgorithmRegistrar().count();
        algorithmWrappers[i].algorithmFileName = algoFilePaths[i];
        if (!algorithmWrappers[i].registerAlgorithm()) return; 
        if (!algorithmWrappers[i].hasAlgorithmRegisteredSuccessfully(countBeforeRegistering)) return;
        std::size_t algorithmIndex = countBeforeRegistering + 1;
        // Zero based
        algorithmWrappers[i].index = algorithmIndex - 1;
    }

    logger.logInfo("All algorithms registered successfully");
}


bool AlgorithmWrapper::createAlgorithm(ThreadController& threadController) {
    auto algorithmRegistered = AlgorithmRegistrar::getAlgorithmRegistrar().begin() + index;
    threadController.setCreatedAlgorithmPointer(algorithmRegistered->create());
    if (!threadController.getCreatedAlgorithmPointer()) {
        isValid = false;
        std::string& algoFilePath = algorithmFileName;
        const std::string errorMessage = "Failed to create algorithm: " + getFileBaseName(algoFilePath);
        threadController.setAlgorithmError(errorMessage);
        threadController.setScore(THREAD_ERROR_CODE);
        return false;
    }    
    return true;
}