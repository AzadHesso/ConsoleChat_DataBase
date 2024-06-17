#include "NetworkConfig.h"
#include <fstream>
#include <iostream>
#include <sstream>

void loadConfigFromFile(const std::string& filename, std::string& ip, int& port) {
    std::ifstream configFile(filename);
    if (!configFile.is_open()) {
        std::cerr << "Unable to open config file: " << filename << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        std::string key;
        if (std::getline(lineStream, key, ':')) {
            std::string value;
            if (std::getline(lineStream, value)) {
                if (key == "IP") {
                    ip = value;
                }
                else if (key == "PORT") {
                    port = std::stoi(value);
                }
            }
        }
    }

    configFile.close();
}
