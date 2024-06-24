#include "Config.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Logger.h"

Config::Config() {
    load("config.txt");
}

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

void Config::load(const std::string& filePath) {
    std::ifstream configFile(filePath);
    if (!configFile) {
        throw std::runtime_error("Unable to open configuration file.");
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        std::string key, value;
        if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
            if (key.empty() || value.empty()) {
                continue;
            }

            configMap[key] = value;
        }
    }
}

const std::string& Config::get(const std::string& key) const {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        return it->second;
    } 

    static std::string defaultValue;
    return defaultValue;
}
