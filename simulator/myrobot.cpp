#include "MySimulator.h"
#include "FileDataExtractor.h"
#include "../common/AlgorithmRegistrar.h"
#include "../common/Logger.h"
#include "../common/Utils.h"
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

namespace fs = std::filesystem;

struct HouseWrapper {
    std::string houseFileName;
    std::mutex mtx;
    bool isValid = true;
};

struct AlgorithmWrapper {
    std::string algorithmFileName;
    void *dlOpenPointer = nullptr;
    std::unique_ptr<AbstractAlgorithm> createdAlgorithmPointer;
    std::size_t index;
    std::mutex mtx;
    bool isValid = true;
};

struct Result {
    std::size_t score;
    bool joined = false;
};

std::tuple<std::ptrdiff_t, bool> parseArguments(int argc, char* argv[], std::string& housePath, std::string& algoPath) {
    Logger& logger = Logger::getInstance();
    logger.logInfo("Parsing arguments");

    std::ptrdiff_t numThreads = DEFAULT_NUM_THREADS_VALUE;
    bool isSummaryOnly = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind(DEFAULT_HOUSE_ARG, 0) == 0) {
            housePath = arg.substr(DEFAULT_HOUSE_ARG.length());
        } else if (arg.rfind(DEFAULT_ALGORITHM_ARG, 0) == 0) {
            algoPath = arg.substr(DEFAULT_ALGORITHM_ARG.length());
        } else if (arg.rfind(DEFAULT_NUM_THREADS_ARG, 0) == 0) {
            numThreads = std::stoul(arg.substr(DEFAULT_NUM_THREADS_ARG.length()));
        } else if (arg.rfind(DEFAULT_SUMMARY_ARG, 0) == 0) {
            isSummaryOnly = true;
        }
    }

    logger.logInfo("Arguments parsed successfully");

    return std::make_tuple(numThreads, isSummaryOnly);
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

void runSimulation(MySimulator& simulator) {
    simulator.run();
}

void createHouseWrappers(const std::vector<std::string>& houseFilePaths, std::vector<HouseWrapper>& houseWrappers) {
    for (std::size_t i = 0; i < houseFilePaths.size(); i++) {
        houseWrappers[i].houseFileName = houseFilePaths[i];
    }
}

bool createAlgorithm(AlgorithmWrapper& algorithmWrapper) {
    auto algorithmRegistered = AlgorithmRegistrar::getAlgorithmRegistrar().begin() + algorithmWrapper.index;
    algorithmWrapper.createdAlgorithmPointer = algorithmRegistered->create();
    if (!algorithmWrapper.createdAlgorithmPointer) {
        std::string& algoFilePath = algorithmWrapper.algorithmFileName;
        const std::string errorMessage = "Failed to create algorithm: " + getFileBaseName(algoFilePath);
        appendToErrorFile(algoFilePath, errorMessage);
        algorithmWrapper.isValid = false;
        return false;
    }    
    return true;
}

bool registerAlgorithm(AlgorithmWrapper& algorithmWrapper) {
    Logger& logger = Logger::getInstance();
    const std::string& algoFilePath = algorithmWrapper.algorithmFileName;
    algorithmWrapper.dlOpenPointer = dlopen(algoFilePath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!algorithmWrapper.dlOpenPointer) {
        const std::string errorMessage = "Failed to dlopen algorithm: " + getFileBaseName(algoFilePath) + " " + std::string(dlerror());
        logger.logError(errorMessage);
        appendToErrorFile(algoFilePath, errorMessage);
        algorithmWrapper.isValid = false;
        return false;
    }
    return true;
}

bool hasAlgorithmRegisteredSuccessfully(AlgorithmWrapper& algorithmWrapper, std::size_t countBeforeRegistering) {
    Logger& logger = Logger::getInstance();
    if (countBeforeRegistering == AlgorithmRegistrar::getAlgorithmRegistrar().count()) {
        algorithmWrapper.isValid = false;
        const std::string& algoFilePath = algorithmWrapper.algorithmFileName;
        const std::string errorMessage = "Failed to register algorithm: " + getFileBaseName(algoFilePath) + " number of algorithms in the registrar is not increased after opening the .so";
        logger.logError(errorMessage);
        appendToErrorFile(algoFilePath, errorMessage);
        return false;
    }
    return true;
}

void createAlgorithmWrappers(const std::vector<std::string>& algoFilePaths, std::vector<AlgorithmWrapper>& algorithmWrappers) {
    Logger& logger = Logger::getInstance();    
    std::size_t i;
    std::size_t countBeforeRegistering;

    logger.logInfo("Registering algorithms");

    for (i = 0; i < algoFilePaths.size(); i++) {
        countBeforeRegistering = AlgorithmRegistrar::getAlgorithmRegistrar().count();
        algorithmWrappers[i].algorithmFileName = algoFilePaths[i];
        if (!registerAlgorithm(algorithmWrappers[i])) return; 
        if (!hasAlgorithmRegisteredSuccessfully(algorithmWrappers[i], countBeforeRegistering)) return;
        std::size_t algorithmIndex = countBeforeRegistering + 1;
        // Zero based
        algorithmWrappers[i].index = algorithmIndex - 1;
    }

    logger.logInfo("All algorithms registered successfully");
}

void generateSummaryCSV(const std::vector<Result>& runResults, std::vector<HouseWrapper>& houseWrappers, std::vector<AlgorithmWrapper>& algorithmWrappers) {
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
            if (houseWrappers[i].isValid) {
                csvFile << "," << getFileBaseName(houseWrappers[i].houseFileName);
            }
        }
        csvFile << "\n";

        for (size_t i = 0; i < numAlgos; i++) {
            if (algorithmWrappers[i].isValid) {
                csvFile << getFileBaseName(algorithmWrappers[i].algorithmFileName);
                for (size_t j = 0; j < numHouses; j++) {
                    if (houseWrappers[j].isValid) {
                        csvFile << "," << runResults[i * numHouses + j].score;
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

    logger.logInfo("Summary file " + DEFAULT_SUMMARY_FILE_NAME + " created successfully");
}

bool initSimulatorFromHouse(MySimulator& simulator, HouseWrapper& houseWrapper, FileDataExtractor& inputData) {
    try {
        inputData.readAndExtract(houseWrapper.houseFileName);
        simulator.initSimulator(inputData, houseWrapper.houseFileName);
    } catch(const std::exception& e) {
        houseWrapper.isValid = false;
        const std::string errorMessage = "House file is not valid: " + houseWrapper.houseFileName + " Due to error: " + e.what();
        appendToErrorFile(houseWrapper.houseFileName, errorMessage);
        return false;
    }
    return true;
}

bool setSimulatorAlgorithm(MySimulator& simulator, AlgorithmWrapper& algorithmWrapper) {
    try {
        simulator.setAlgorithm(*algorithmWrapper.createdAlgorithmPointer, algorithmWrapper.algorithmFileName);
    } catch(const std::exception& e) {
        const std::string errorMessage = "Failed to set algorithm: " + algorithmWrapper.algorithmFileName + "Due to error: " + e.what();
        appendToErrorFile(algorithmWrapper.algorithmFileName, errorMessage);
        return false;
    }
    return true;
}

void workerMonitor(HouseWrapper& houseWrapper, AlgorithmWrapper& algorithmWrapper, Result& result, bool isSummaryOnly) {
    std::lock_guard<std::mutex> lockHouse(houseWrapper.mtx);
    MySimulator simulator;  
    FileDataExtractor inputData = FileDataExtractor();
    bool isValidHouse = initSimulatorFromHouse(simulator, houseWrapper, inputData);
    if (!isValidHouse) {
        result.score = THREAD_ERROR_CODE;
        return;
    }

    std::lock_guard<std::mutex> lockAlgo(algorithmWrapper.mtx);
    bool isCreated = createAlgorithm(algorithmWrapper);
    if (!isCreated) {
        result.score = THREAD_ERROR_CODE;
        return;
    }

    bool isSet = setSimulatorAlgorithm(simulator, algorithmWrapper);
    if (!isSet) {
        result.score = THREAD_ERROR_CODE;
        return;
    }

    simulator.setIsSummaryOnly(isSummaryOnly);
    result.score = simulator.getMaxSteps() * 2 + simulator.getTotalDirt() * 300 + 2000;
    
    std::string houseFileBaseName = simulator.getHouseName();
    std::string algorithmFileBaseName = simulator.getAlgorithmName();

    try {
        auto worker = [&simulator, &houseFileBaseName, &algorithmFileBaseName]() {
            try {
                runSimulation(simulator);
            }
            catch(const std::exception& e) {
                const std::string errorMessage = "Running simulation for house " + houseFileBaseName + " and algorithm " + algorithmFileBaseName + " has failed due to error: " + e.what();
                appendToErrorFile(simulator.getAlgorithmName(), errorMessage);
            }
        };

        std::chrono::milliseconds timeout(simulator.getMaxSteps());

        std::packaged_task<void()> task(worker);
        std::future<void> future = task.get_future();
        std::thread workerThread(std::move(task));
        throw std::runtime_error("An unknown unrecoverable error occurred.");
        if (future.wait_for(timeout) == std::future_status::timeout) {
            workerThread.detach();
            return;
        }

        workerThread.join();
        result.score = simulator.getScore();
    }
    catch (const std::exception& e) {
        appendToErrorFile(algorithmWrapper.algorithmFileName, "An error has occurred during monitor of simulation for house " + houseFileBaseName + " and algorithm " + algorithmFileBaseName + ". The error is: " + e.what());
    }
    catch (...) {
        appendToErrorFile(algorithmWrapper.algorithmFileName, "An unknown error has occurred during monitor of simulation for house " + houseFileBaseName + " and algorithm " + algorithmFileBaseName);
    }
}

int main(int argc, char *argv[]) {
    Logger& logger = Logger::getInstance();
    size_t i, j;
    std::string housePath = DEFAULT_HOUSE_DIR_PATH;
    std::string algoPath = DEFAULT_ALGORITHM_DIR_PATH;
    std::ptrdiff_t numThreads;
    std::size_t threadNumber = 0;
    std::vector<std::string> houseFilePaths;
    std::vector<std::string> algoFilePaths;
    bool isSummaryOnly = false;

    try {
        std::tie(numThreads, isSummaryOnly) = parseArguments(argc, argv, housePath, algoPath);

        findHouseFiles(housePath, houseFilePaths);
        findAlgoFiles(algoPath, algoFilePaths);

        size_t numHouses = houseFilePaths.size();
        size_t numAlgos = algoFilePaths.size();

        std::vector<HouseWrapper> houseWrappers(numHouses);
        std::vector<AlgorithmWrapper> algorithmWrappers(numAlgos);

        createHouseWrappers(houseFilePaths, houseWrappers);
        createAlgorithmWrappers(algoFilePaths, algorithmWrappers);

        std::vector<std::thread> workerMonitorThreads;
        std::vector<Result> runResults(numHouses * numAlgos);
        std::counting_semaphore semaphore{numThreads};

        logger.logInfo("Start creating threads");
        
        for (j = 0; j < numAlgos; j++) {
            for (i = 0; i < numHouses; i++) {
                threadNumber = j * numHouses + i;
                runResults[threadNumber].score = THREAD_ERROR_CODE;

                if (!houseWrappers[i].isValid || !algorithmWrappers[j].isValid) {
                    workerMonitorThreads.emplace_back();
                    runResults[threadNumber].joined = true;
                    continue;
                }
                
                semaphore.acquire();
                logger.logInfo("--------Creating thread number " + std::to_string(threadNumber) + "--------");
                logger.logInfo("House file: " + getFileBaseName(houseWrappers[i].houseFileName));
                logger.logInfo("Algorithm file: " + getFileBaseName(algorithmWrappers[j].algorithmFileName));
                workerMonitorThreads.emplace_back([&semaphore, &houseWrappers, &algorithmWrappers, &runResults, i, j, threadNumber, isSummaryOnly]() {
                    workerMonitor(houseWrappers[i], algorithmWrappers[j], runResults[threadNumber], isSummaryOnly);
                    semaphore.release();
                });
            }
        }

        logger.logInfo("All threads created successfully");
        
        logger.logInfo("Start joining threads");
        for (i = 0; i < workerMonitorThreads.size(); i++) {
            if (runResults[i].joined) {
                continue;
            }
            workerMonitorThreads[i].join();
            runResults[i].joined = true;
        }
        logger.logInfo("All threads joined successfully");

        generateSummaryCSV(runResults, houseWrappers, algorithmWrappers);

        logger.logInfo("Clearing algorithm registrar");
        AlgorithmRegistrar::getAlgorithmRegistrar().clear();

        logger.logInfo("Start dlclose algorithm files");
        for (auto& algoWrapper : algorithmWrappers) {
            if (algoWrapper.dlOpenPointer) {
                algoWrapper.createdAlgorithmPointer.reset();
                dlclose(algoWrapper.dlOpenPointer);
                break;
            }
        }
        logger.logInfo("All algorithm files dlclosed successfully");
    } catch (const std::exception& e) {
        handleMainThreadException(e);
        return 1;
    } catch (...) {
        handleMainThreadException(std::runtime_error("An unknown unrecoverable error occurred."));
        return 1;
    }

    return 0;
}