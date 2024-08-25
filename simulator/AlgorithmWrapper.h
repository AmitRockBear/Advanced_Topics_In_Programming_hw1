#ifndef ALGORITHM_WRAPPER_H
#define ALGORITHM_WRAPPER_H

#include <string>
#include <dlfcn.h>
#include "../common/AlgorithmRegistrar.h"
#include "ThreadController.h"
#include <atomic>
#include <mutex>

class AlgorithmWrapper {
public:
    AlgorithmWrapper() = default;
    AlgorithmWrapper(const std::string& algorithmFileName);
    AlgorithmWrapper(const AlgorithmWrapper&) = delete;
    AlgorithmWrapper& operator=(const AlgorithmWrapper&) = delete;
    AlgorithmWrapper(AlgorithmWrapper&&) = delete;
    AlgorithmWrapper& operator=(AlgorithmWrapper&&) = delete;
    ~AlgorithmWrapper();

    const std::string& getAlgorithmFileName() const { return algorithmFileName; }
    void* getDlOpenPointer() const { return dlOpenPointer; }
    std::size_t getIndex() const { return index; }
    bool getIsValid() const { return isValid; }
    int getUsageCounter() const { return usageCounter.load(); }
    std::mutex& getMtx() { return mtx; }

    void setAlgorithmFileName(const std::string& fileName) { algorithmFileName = fileName; }
    void setDlOpenPointer(void* pointer) { dlOpenPointer = pointer; }
    void setIndex(std::size_t idx) { index = idx; }
    void setIsValid(bool valid) { isValid = valid; }
    void incrementUsageCounter() { ++usageCounter; }
    void decrementUsageCounter() { --usageCounter; }


    static void createAlgorithmWrappers(const std::vector<std::string>& algoFilePaths, std::vector<AlgorithmWrapper>& algorithmWrappers);
    bool createAlgorithm(ThreadController& threadController);

private:
    std::string algorithmFileName;
    void* dlOpenPointer = nullptr;
    std::size_t index = 0;
    bool isValid = true;
    std::atomic<int> usageCounter{0};
    std::mutex mtx;

    bool registerAlgorithm();
    bool hasAlgorithmRegisteredSuccessfully(std::size_t countBeforeRegistering);
};

#endif
