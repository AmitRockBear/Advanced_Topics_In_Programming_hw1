#include "Config.h"
#include "General.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

Config::Config() {
    load("config.txt");
}

void Config::load(const std::string& filePath) {
    std::ifstream configFile(filePath);
    if (!configFile) {
        throw std::runtime_error("Unable to open configuration file.");
    }
    try {
        std::string line;
        std::string key, value;
        while (std::getline(configFile, line)) {
            bool extractedSuccessfully = extractKeyValue(line, key, value);
            if (!extractedSuccessfully) continue;
            configMap[key] = value;
        }
    } catch (...) {
        configFile.close();
        throw;
    }
}

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

const std::string& Config::get(const std::string& key) const {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        return it->second;
    } 

    return EMPTY_STRING;
}
