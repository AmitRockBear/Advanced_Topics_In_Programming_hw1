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

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        std::string key, value;
        if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
            key = trim(key);
            value = trim(value);

            if (key.empty() || value.empty()) {
                continue;
            }

            configMap[key] = value;
        }
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
