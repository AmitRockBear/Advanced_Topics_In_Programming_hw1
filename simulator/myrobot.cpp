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
    // std::cout << "algoFilePath: " << algoFilePath << std::endl;
    // std::cout << "dlOpenPointer: " << algorithmWrapper.dlOpenPointer << std::endl;
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

void generateSummaryCSV(const std::vector<Result>& runResults, std::vector<HouseWrapper>& houseWrappers, std::vector<AlgorithmWrapper>& algorithmWrappers) {
    size_t numHouses = houseWrappers.size();
    size_t numAlgos = algorithmWrappers.size();
    
    std::ofstream csvFile(DEFAULT_SUMMARY_FILE_NAME);

    if (!csvFile.is_open()) {
        // std::cerr << "Failed to open " << DEFAULT_SUMMARY_FILE_NAME << " for writing." << std::endl;
        Logger::getInstance().logError("Failed to open " + DEFAULT_SUMMARY_FILE_NAME + " for writing.");
        return;
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
        // std::cout << "ERROR: Algorithm" << algorithmWrapper.algorithmFileName << " is not created" << std::endl;
        result.score = THREAD_ERROR_CODE;
        return;
    }

    std::lock_guard<std::mutex> lockSim(houseWrapper.mtx);
    // std::cout << "Locked both locks house name is" << houseWrapper.houseFileName << " algorithm name is " << algorithmWrapper.algorithmFileName << std::endl;
    const std::string& houseFilePath = houseWrapper.houseFileName;
    MySimulator simulator;  
    FileDataExtractor inputData = FileDataExtractor();
    bool isValidHouse = inputData.readAndExtract(houseWrapper.houseFileName);
    if (!isValidHouse) {
        // std::cout << "ERROR: House is not valid" << houseWrapper.houseFileName << std::endl;
        houseWrapper.isValid = false;
        const std::string errorMessage = "House file is not valid: " + getFileBaseName(houseFilePath);
        createErrorFile(houseFilePath, errorMessage);
        result.score = THREAD_ERROR_CODE;
        return;
    }

    try {
        simulator.initSimulator(inputData, houseFilePath, isSummaryOnly);
        result.score = simulator.getMaxSteps() * 2 + simulator.getTotalDirt() * 300 + 2000;
        // std::cout << "House is " << houseWrapper.houseFileName << " Algorithm is " << algorithmWrapper.algorithmFileName <<  "My result.score is " << result.score << std::endl;
        simulator.setAlgorithm(*algorithmWrapper.createdAlgorithmPointer, algorithmWrapper.algorithmFileName);
        auto worker = [&simulator]() {
            while(1) {}
            runSimulation(simulator);
        };

        std::chrono::milliseconds timeout(simulator.getMaxSteps() * 1);

        std::packaged_task<void()> task(worker);
        std::future<void> future = task.get_future();
        std::thread workerThread(std::move(task));

        if (future.wait_for(timeout) == std::future_status::timeout) {
            workerThread.detach();
            return;
        }

        // std::cout << "Thread " << workerThread.get_id() << " is has finished House is " << houseWrapper.houseFileName << " Algorithm is " << algorithmWrapper.algorithmFileName << std::endl;
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

        std::vector<std::thread> workerWrapperThreads;
        std::vector<Result> runResults(numHouses * numAlgos);
        std::counting_semaphore semaphore{numThreads};

        for (size_t j = 0; j < numAlgos; j++) {
            for (size_t i = 0; i < numHouses; i++) {
                threadNumber = j * numHouses + i;
                runResults[threadNumber].score = THREAD_ERROR_CODE;

                if (!houseWrappers[i].isValid || !algorithmWrappers[j].isValid) {
                    workerWrapperThreads.emplace_back();
                    runResults[threadNumber].joined = true;
                    continue;
                }
                
                semaphore.acquire();

                workerWrapperThreads.emplace_back([&semaphore, &houseWrappers, &algorithmWrappers, &runResults, i, j, threadNumber, isSummaryOnly]() {
                    workerWrapper(houseWrappers[i], algorithmWrappers[j], runResults[threadNumber], isSummaryOnly);
                    semaphore.release();
                });
            }
        }
        
        for (std::size_t i = 0; i < workerWrapperThreads.size(); i++) {
            if (runResults[i].joined) {
                continue;
            }
            workerWrapperThreads[i].join();
            runResults[i].joined = true;
        }

        generateSummaryCSV(runResults, houseWrappers, algorithmWrappers);

        AlgorithmRegistrar::getAlgorithmRegistrar().clear();

        for (auto& algoWrapper : algorithmWrappers) {
            if (algoWrapper.dlOpenPointer) {
                // std::cout << "dlOpenPointer: " << algoWrapper.dlOpenPointer 
                        // << " createdAlgorithmPointer: " << &algoWrapper.createdAlgorithmPointer 
                        // << " algorithmFileName: " << algoWrapper.algorithmFileName << std::endl;

                // std::cout << "Resetting createdAlgorithmPointer..." << std::endl;
                algoWrapper.createdAlgorithmPointer.reset();
                // std::cout << "Reset complete." << std::endl;

                // std::cout << "before dlclose" << std::endl;
                dlclose(algoWrapper.dlOpenPointer);
                // std::cout << "after dlclose" << std::endl;
                break;
            }
        }
    } catch (const std::exception& e) {
        handleException(e);
        return 1;
    } catch (...) {
        handleException(std::runtime_error("An unknown unrecoverable error occurred."));
        return 1;
    }

    return 0;
}