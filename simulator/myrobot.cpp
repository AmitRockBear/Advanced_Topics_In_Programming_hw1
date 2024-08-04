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
    std::string houseFileName;
    std::string algorithmFileName;
    std::size_t score;
    bool joined = false;
};

std::tuple<std::size_t, bool> parseArguments(int argc, char* argv[], std::string& housePath, std::string& algoPath) {
    std::size_t numThreads = DEFAULT_NUM_THREADS_VALUE;
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

    return std::make_tuple(numThreads, isSummaryOnly);
}

void findHouseFiles(const std::string& housePath, std::vector<std::string>& houseFilePaths) {
    for (const auto& entry : fs::directory_iterator(housePath)) {
        if (entry.is_regular_file() && entry.path().extension() == DEFAULT_HOUSE_FILE_EXTENSION) {
            houseFilePaths.push_back(entry.path().string());
        }
    }
}

void findAlgoFiles(const std::string& algoPath, std::vector<std::string>& algoFilePaths) {
    for (const auto& entry : fs::directory_iterator(algoPath)) {
        if (entry.is_regular_file() && entry.path().extension() == DEFAULT_ALGORITHM_FILE_EXTENSION) {
            algoFilePaths.push_back(entry.path().string());
        }
    }
}

void runSimulation(MySimulator& simulator) {
    simulator.run();
}

void setThreadPriorityToIdle(std::thread& thread) {
    pthread_t handle = thread.native_handle();
    sched_param sch_params;
    sch_params.sched_priority = 0; // Lower priority

    int policy;
    pthread_getschedparam(handle, &policy, &sch_params);
    policy = SCHED_IDLE;

    if (pthread_setschedparam(handle, policy, &sch_params)) {
        std::cerr << "Failed to set thread priority to IDLE." << std::endl;
    }
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
        createErrorFile(algoFilePath, errorMessage);
        algorithmWrapper.isValid = false;
        return false;
    }    
    return true;
}

bool registerAlgorithm(AlgorithmWrapper& algorithmWrapper) {
    const std::string& algoFilePath = algorithmWrapper.algorithmFileName;
    algorithmWrapper.dlOpenPointer = dlopen(algoFilePath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!algorithmWrapper.dlOpenPointer) {
        const std::string errorMessage = "Failed to dlopen algorithm: " + getFileBaseName(algoFilePath) + " " + std::string(dlerror());
        createErrorFile(algoFilePath, errorMessage);
        algorithmWrapper.isValid = false;
        return false;
    }
    return true;
}

bool hasAlgorithmRegisteredSuccessfully(AlgorithmWrapper& algorithmWrapper, std::size_t countBeforeRegistering) {
    if (countBeforeRegistering == AlgorithmRegistrar::getAlgorithmRegistrar().count()) {
        algorithmWrapper.isValid = false;
        const std::string& algoFilePath = algorithmWrapper.algorithmFileName;
        const std::string errorMessage = "Failed to register algorithm: " + getFileBaseName(algoFilePath) + " number of algorithms in the registrar is not increased after opening the .so";
        createErrorFile(algoFilePath, errorMessage);
        return false;
    }
    return true;
}

void createAlgorithmWrappers(const std::vector<std::string>& algoFilePaths, std::vector<AlgorithmWrapper>& algorithmWrappers) {
    std::size_t i;
    std::size_t countBeforeRegistering;
    for (i = 0; i < algoFilePaths.size(); i++) {
        countBeforeRegistering = AlgorithmRegistrar::getAlgorithmRegistrar().count();
        algorithmWrappers[i].algorithmFileName = algoFilePaths[i];
        if (!registerAlgorithm(algorithmWrappers[i])) return; 
        if (!hasAlgorithmRegisteredSuccessfully(algorithmWrappers[i], countBeforeRegistering)) return;
        std::size_t algorithmIndex = countBeforeRegistering + 1;
        // Zero based
        algorithmWrappers[i].index = algorithmIndex - 1;
    }
}

void generateSummaryCSV(const std::vector<Result>& runResults, size_t numHouses, size_t numAlgos) {
    std::ofstream csvFile(DEFAULT_SUMMARY_FILE_NAME);

    if (!csvFile.is_open()) {
        // std::cerr << "Failed to open " << DEFAULT_SUMMARY_FILE_NAME << " for writing." << std::endl;
        Logger::getInstance().logError("Failed to open " + DEFAULT_SUMMARY_FILE_NAME + " for writing.");
        return;
    }
    try
    {
        csvFile << "Algorithm/House";
        for (std::size_t i = 0; i < runResults.size(); i += numAlgos) {
            csvFile << "," << runResults[i].houseFileName;
        }
        csvFile << "\n";

        for (size_t i = 0; i < numAlgos; i++) {
            csvFile << runResults[i * numHouses].algorithmFileName;
            for (size_t j = 0; j < numHouses; j++) {
                csvFile << "," << runResults[i * numHouses + j].score;
            }
            csvFile << "\n";
        }

        csvFile.close();
    }
    catch(const std::exception& e)
    {
        csvFile.close();
        handleException(e);

    }
}

void workerWrapper(HouseWrapper& houseWrapper, AlgorithmWrapper& algorithmWrapper, Result& result, bool isSummaryOnly) {
    std::lock_guard<std::mutex> lockAlgo(algorithmWrapper.mtx);
    bool isCreated = createAlgorithm(algorithmWrapper);
    if (!isCreated) {
        result.score = THREAD_ERROR_CODE;
        return;
    }

    std::lock_guard<std::mutex> lockSim(houseWrapper.mtx);
    const std::string& houseFilePath = houseWrapper.houseFileName;
    MySimulator simulator;  
    FileDataExtractor inputData = FileDataExtractor();
    bool isValidHouse = inputData.readAndExtract(houseWrapper.houseFileName);
    if (!isValidHouse) {
        houseWrapper.isValid = false;
        const std::string errorMessage = "House file is not valid: " + getFileBaseName(houseFilePath);
        createErrorFile(houseFilePath, errorMessage);
        result.score = THREAD_ERROR_CODE;
        return;
    }

    try {
        simulator.initSimulator(inputData, houseFilePath, isSummaryOnly);
        result.score = simulator.getMaxSteps() * 2 + simulator.getTotalDirt() * 300 + 2000;
        simulator.setAlgorithm(*algorithmWrapper.createdAlgorithmPointer, algorithmWrapper.algorithmFileName);
        auto worker = [&simulator]() {
            runSimulation(simulator);
        };

        std::chrono::milliseconds timeout(simulator.getMaxSteps() * 1);

        std::packaged_task<void()> task(worker);
        std::future<void> future = task.get_future();
        std::thread workerThread(std::move(task));

        if (future.wait_for(timeout) == std::future_status::timeout) {
            setThreadPriorityToIdle(workerThread);
            workerThread.join();
            return;
        }
        workerThread.join();
        result.score = simulator.getScore();
    }
    catch(const std::exception& e) {
        handleException(e);
    }
}

int main(int argc, char *argv[]) {
    std::string housePath = DEFAULT_HOUSE_DIR_PATH;
    std::string algoPath = DEFAULT_ALGORITHM_DIR_PATH;
    std::size_t numThreads;
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

        std::vector<std::thread> workerWrapperThreads;
        std::vector<Result> runResults(numHouses * numAlgos);

        for (size_t j = 0; j < numAlgos; j++) {
            for (size_t i = 0; i < numHouses; i++) {
                threadNumber = j * numHouses + i;
                runResults[threadNumber].houseFileName = getFileBaseName(houseWrappers[i].houseFileName);
                runResults[threadNumber].algorithmFileName = getFileBaseName(algorithmWrappers[j].algorithmFileName);
                runResults[threadNumber].score = THREAD_ERROR_CODE;

                if (!houseWrappers[i].isValid || !algorithmWrappers[j].isValid) {
                    workerWrapperThreads.emplace_back();
                    runResults[threadNumber].joined = true;
                    continue;
                }

                workerWrapperThreads.emplace_back(workerWrapper, std::ref(houseWrappers[i]), std::ref(algorithmWrappers[j]), std::ref(runResults[threadNumber]), isSummaryOnly);
                
                // Limit the number of concurrent threads to numThreads
                if ((threadNumber + 1) % numThreads == 0) {
                    for (std::size_t l = numThreads - 1;; l--) {
                        workerWrapperThreads[threadNumber - l].join();
                        runResults[threadNumber - l].joined = true;
                    }
                }
            }
        }
        
        for (std::size_t i = 0; i < workerWrapperThreads.size(); i++) {
            if (runResults[i].joined) {
                continue;
            }
            workerWrapperThreads[i].join();
            runResults[i].joined = true;
        }

        generateSummaryCSV(runResults, numHouses, numAlgos);

        
        // for (auto& algoWrapper : algorithmWrappers) {
        //     if (algoWrapper.dlOpenPointer) {
        //         std::cout << "dlOpenPointer: " << algoWrapper.dlOpenPointer << " createdAlgorithmPointer: " << &algoWrapper.createdAlgorithmPointer << " algorithmFileName: " << algoWrapper.algorithmFileName << std::endl;
        //         std::cout << "before dlclose" << std::endl;
        //         dlclose(algoWrapper.dlOpenPointer);
        //         std::cout << "after dlclose" << std::endl;
        //     }
        // }
    } catch (const std::exception& e) {
        handleException(e);
        return 1;
    } catch (...) {
        handleException(std::runtime_error("An unknown unrecoverable error occurred."));
        return 1;
    }

    return 0;
}