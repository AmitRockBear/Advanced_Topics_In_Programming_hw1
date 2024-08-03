#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

class Config {
public:
    static Config& getInstance();
    const std::string& get(const std::string& key) const;

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;
private:
    Config();
    ~Config() = default;
    void load(const std::string& filePath);

    std::unordered_map<std::string, std::string> configMap;
};

#endif
