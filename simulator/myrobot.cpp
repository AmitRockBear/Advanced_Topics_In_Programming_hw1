#include "MySimulator.h"
#include "FileDataExtractor.h"
#include "../common/AlgorithmRegistrar.h"
#include "../common/Logger.h"
#include "../common/Utils.h"
#include "../common/General.h"
#include "AlgorithmWrapper.h"
#include "HouseWrapper.h"
#include "ThreadController.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <mutex>
#include <sched.h>
#include <dlfcn.h>
#include <future>
#include <semaphore>
#include <condition_variable>
#include <atomic>
#include <mutex>


namespace fs = std::filesystem;

struct ParseResult {
    std::ptrdiff_t numThreads;
    bool isSummaryOnly;
};

ParseResult parseArguments(int argc, char* argv[], std::string& housePath, std::string& algoPath) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Parsing arguments");

    std::ptrdiff_t numThreads = DEFAULT_NUM_THREADS_VALUE;
    bool isSummaryOnly = false;
    ParseResult result{numThreads, isSummaryOnly};

    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg.rfind(DEFAULT_HOUSE_ARG, 0) == 0) {
                housePath = arg.substr(DEFAULT_HOUSE_ARG.length());
            } else if (arg.rfind(DEFAULT_ALGORITHM_ARG, 0) == 0) {
                algoPath = arg.substr(DEFAULT_ALGORITHM_ARG.length());
            } else if (arg.rfind(DEFAULT_NUM_THREADS_ARG, 0) == 0) {
                result.numThreads = static_cast<std::ptrdiff_t>(std::stoul(arg.substr(DEFAULT_NUM_THREADS_ARG.length())));
            } else if (arg.rfind(DEFAULT_SUMMARY_ARG, 0) == 0) {
                result.isSummaryOnly = true;
            }
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error during parsing arguments: " + std::string(e.what()));
    }
    catch (...) {
        throw std::runtime_error("Unknown error during parsing arguments");
    }

    logger.logInfo("Arguments parsed successfully");

    return result;
}

void findHouseFiles(const std::string& housePath, std::vector<std::string>& houseFilePaths) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Finding house files");

    for (const auto& entry : fs::directory_iterator(housePath)) {
        if (entry.is_regular_file() && entry.path().extension() == DEFAULT_HOUSE_FILE_EXTENSION) {
            houseFilePaths.push_back(entry.path().string());
        }
    }
    
    logger.logInfo("All house files found successfully");
}

void findAlgoFiles(const std::string& algoPath, std::vector<std::string>& algoFilePaths) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Finding algorithm files");

    for (const auto& entry : fs::directory_iterator(algoPath)) {
        if (entry.is_regular_file() && entry.path().extension() == DEFAULT_ALGORITHM_FILE_EXTENSION) {
            algoFilePaths.push_back(entry.path().string());
        }
    }

    logger.logInfo("All algorithm files found successfully");
}

void generateSummaryCSV(const std::vector<ThreadController>& threadControllers, std::vector<HouseWrapper>& houseWrappers, std::vector<AlgorithmWrapper>& algorithmWrappers) {
    Logger& logger = Logger::getInstance();    
    size_t numHouses = houseWrappers.size();
    size_t numAlgos = algorithmWrappers.size();
    
    logger.logInfo("Start creating " + DEFAULT_SUMMARY_FILE_NAME + " file");

    std::ofstream csvFile(DEFAULT_SUMMARY_FILE_NAME);

    if (!csvFile.is_open()) {
        throw std::runtime_error("Failed to open " + DEFAULT_SUMMARY_FILE_NAME + " for writing.");
    }
    try {
        csvFile << "Algorithm/House";
        for (std::size_t i = 0; i < numHouses; i++) {
            if (houseWrappers[i].getIsValid()) {
                csvFile << "," << getFileBaseName(houseWrappers[i].getHouseFileName());
            }
        }
        csvFile << "\n";

        for (size_t i = 0; i < numAlgos; i++) {
            if (algorithmWrappers[i].getIsValid()) {
                csvFile << getFileBaseName(algorithmWrappers[i].getAlgorithmFileName());
                for (size_t j = 0; j < numHouses; j++) {
                    if (houseWrappers[j].getIsValid()) {
                        csvFile << "," << threadControllers[i * numHouses + j].getScore();
                    }
                }
                csvFile << "\n"; 
            }
        }

        csvFile.close();    
    }
    catch (const std::exception& e) {
        csvFile.close();
        throw std::runtime_error("Error during writing to summary file: " + DEFAULT_SUMMARY_FILE_NAME + ". The error is: " + e.what());
    }
    catch (...) {
        csvFile.close();
        throw std::runtime_error("Unknown error during writing to summary file: " + DEFAULT_SUMMARY_FILE_NAME);
    }

    logger.logInfo("Summary file " + DEFAULT_SUMMARY_FILE_NAME + " created successfully");
}

void writeErrorsToFiles(const std::vector<ThreadController>& threadControllers, std::vector<HouseWrapper>& houseWrappers, std::vector<AlgorithmWrapper>& algorithmWrappers) {
    Logger& logger = Logger::getInstance();    
    std::size_t index;
    size_t numHouses = houseWrappers.size();
    size_t numAlgos = algorithmWrappers.size();
    logger.logInfo("Start writing errors to files");

    try {
        for (size_t i = 0; i < numAlgos; i++) {
            for (size_t j = 0; j < numHouses; j++) {
                index = i * numHouses + j;
                const std::string houseError = threadControllers[index].getHouseError();
                const std::string algorithmError = threadControllers[index].getAlgorithmError();
                
                if (!houseError.empty()) {
                    appendToErrorFile(threadControllers[index].getHouseFileBaseName(), houseError);
                }
                if (!algorithmError.empty()) {
                    appendToErrorFile(threadControllers[index].getAlgorithmFileBaseName(), algorithmError);
                }
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error during writing errors to files: " + std::string(e.what()));
    }
    catch (...) {
        throw std::runtime_error("Unknown error during writing errors to files");
    }

    logger.logInfo("All errors were written to files successfully");
}

void workerMonitor(HouseWrapper& houseWrapper, AlgorithmWrapper& algorithmWrapper, ThreadController& threadController, bool isSummaryOnly) {
    MySimulator simulator;  
    FileDataExtractor inputData = FileDataExtractor();
    bool isValidHouse = simulator.initSimulatorFromHouse(threadController, houseWrapper, inputData);
    if (!isValidHouse) {
        return;
    }

    bool isCreated = algorithmWrapper.createAlgorithm(threadController);
    if (!isCreated) {
        return;
    }

    const std::string algorithmFileName = algorithmWrapper.getAlgorithmFileName();
    bool isSet = simulator.setSimulatorAlgorithm(threadController, algorithmFileName);
    if (!isSet) {
        return;
    }
    
    std::size_t score = simulator.getMaxSteps() * 2 + simulator.getTotalDirt() * 300 + 2000;
    simulator.setScore(score);
    threadController.setScore(score);
    std::atomic<bool> workerFailed(false);
    const std::string houseFileBaseName = threadController.getHouseFileBaseName();
    const std::string algorithmFileBaseName = threadController.getAlgorithmFileBaseName();
    try {
        auto worker = [&simulator, &threadController, &workerFailed, &houseFileBaseName, &algorithmFileBaseName]() { 
            try {
                simulator.run();
            }
            catch(const std::exception& e) {
                threadController.setAlgorithmError("Running simulation for house " + houseFileBaseName + " and algorithm " + algorithmFileBaseName + " has failed due to error: " + e.what());
                workerFailed.store(true);
            }
            catch(...) {
                threadController.setAlgorithmError("Running simulation for house " + houseFileBaseName + " and algorithm " + algorithmFileBaseName + " has failed due to unknown error.");
                workerFailed.store(true);
            }
        };

        std::chrono::milliseconds timeout(simulator.getMaxSteps());

        std::packaged_task<void()> task(worker);
        std::future<void> future = task.get_future();
        std::thread workerThread(std::move(task));
        if (future.wait_for(timeout) == std::future_status::timeout) {
            threadController.setAlgorithmError("Running simulation for house " + houseFileBaseName + " and algorithm " + algorithmFileBaseName + " has failed due to timeout.");
            if (!isSummaryOnly) simulator.createOutputFile(true);
            workerThread.detach();
            return;
        }
        workerThread.join();
        
        if (!isSummaryOnly) { 
            simulator.createOutputFile(false);
        }
        if (!workerFailed.load()) {
            threadController.setScore(simulator.getScore());
        }
    }
    catch (const std::exception& e) {
        threadController.setAlgorithmError("An error has occurred during monitor of simulation for house " + houseFileBaseName + " and algorithm " + algorithmFileBaseName + ". The error is: " + e.what());
    }
    catch (...) {
        threadController.setAlgorithmError("An unknown error has occurred during monitor of simulation for house " + houseFileBaseName + " and algorithm " + algorithmFileBaseName);
    }
}

int main(int argc, char *argv[]) {
    Logger& logger = Logger::getInstance();
    std::size_t i, j;
    std::string housePath = DEFAULT_HOUSE_DIR_PATH;
    std::string algoPath = DEFAULT_ALGORITHM_DIR_PATH;
    std::ptrdiff_t numThreads;
    std::size_t threadNumber = 0;
    std::vector<std::string> houseFilePaths;
    std::vector<std::string> algoFilePaths;
    bool isSummaryOnly = false;
    ParseResult parseResult;

    try {
        parseResult = parseArguments(argc, argv, housePath, algoPath);
        numThreads = parseResult.numThreads;
        isSummaryOnly = parseResult.isSummaryOnly;
        
        findHouseFiles(housePath, houseFilePaths);
        findAlgoFiles(algoPath, algoFilePaths);

        size_t numHouses = houseFilePaths.size();
        size_t numAlgos = algoFilePaths.size();

        std::vector<HouseWrapper> houseWrappers(numHouses);
        std::vector<AlgorithmWrapper> algorithmWrappers(numAlgos);

        HouseWrapper::createHouseWrappers(houseFilePaths, houseWrappers);
        AlgorithmWrapper::createAlgorithmWrappers(algoFilePaths, algorithmWrappers);

        std::vector<std::thread> workerMonitorThreads;
        std::vector<ThreadController> threadControllers(numHouses * numAlgos);
        std::counting_semaphore semaphore{numThreads};

        logger.logInfo("Start creating threads");
        
        for (j = 0; j < numAlgos; j++) {
            for (i = 0; i < numHouses; i++) {
                threadNumber = j * numHouses + i;
                threadControllers[threadNumber].setScore(THREAD_ERROR_CODE);
                threadControllers[threadNumber].setHouseFileBaseName(getFileBaseName(houseWrappers[i].getHouseFileName()));
                threadControllers[threadNumber].setAlgorithmFileBaseName(getFileBaseName(algorithmWrappers[j].getAlgorithmFileName()));

                if (!houseWrappers[i].getIsValid() || !algorithmWrappers[j].getIsValid()) {
                    workerMonitorThreads.emplace_back();
                    threadControllers[threadNumber].setJoined(true);
                    continue;
                }
                
                semaphore.acquire();
                logger.logInfo("--------Creating thread number " + std::to_string(threadNumber) + "--------");
                logger.logInfo("House file: " + threadControllers[threadNumber].getHouseFileBaseName());
                logger.logInfo("Algorithm file: " + threadControllers[threadNumber].getAlgorithmFileBaseName());
                workerMonitorThreads.emplace_back([&semaphore, &houseWrappers, &algorithmWrappers, &threadControllers, i, j, threadNumber, isSummaryOnly]() {
                    {
                        std::lock_guard<std::mutex> lock(algorithmWrappers[j].getMtx());
                        algorithmWrappers[j].incrementUsageCounter();
                    }
                    workerMonitor(houseWrappers[i], algorithmWrappers[j], threadControllers[threadNumber], isSummaryOnly);
                    {
                        std::lock_guard<std::mutex> lock(algorithmWrappers[j].getMtx());
                        algorithmWrappers[j].decrementUsageCounter();
                    }
                    semaphore.release();
                });
            }
        }
        logger.logInfo("All threads created successfully");
        
        logger.logInfo("Start joining threads");
        for (i = 0; i < workerMonitorThreads.size(); i++) {
            if (threadControllers[i].isJoined()) {
                continue;
            }
            workerMonitorThreads[i].join();
            threadControllers[i].setJoined(true);
        }
        logger.logInfo("All threads joined successfully");

        writeErrorsToFiles(threadControllers, houseWrappers, algorithmWrappers);
        generateSummaryCSV(threadControllers, houseWrappers, algorithmWrappers);

        logger.logInfo("Clearing algorithm registrar");
        AlgorithmRegistrar::getAlgorithmRegistrar().clear();

        logger.logInfo("Resetting created algorithm pointers");
        for (auto& threadController : threadControllers) {
            if (threadController.getCreatedAlgorithmPointer()) {
                threadController.resetCreatedAlgorithmPointer();
            }
        }

        logger.logInfo("Start dlclose algorithm files");
        for (auto& algoWrapper : algorithmWrappers) {
            void* dlOpenPointer = algoWrapper.getDlOpenPointer();
            if (dlOpenPointer && algoWrapper.getUsageCounter() == 0) {
                dlclose(dlOpenPointer);
            }
        }
        logger.logInfo("All algorithm files that were allowed for dlclose, dlclosed successfully");

    } catch (const std::exception& e) {
        handleMainThreadException(e);
        return 1;
    } catch (...) {
        handleMainThreadException(std::runtime_error("An unknown unrecoverable error occurred."));
        return 1;
    }

    return 0;
}