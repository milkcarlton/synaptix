#include "loader.h"

#include <sys/capability.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <pwd.h>

Loader::Loader(DiskUtils* disk) {
    this->disk = disk;
}

Loader::~Loader() { }

unsigned short safeStrToNum(std::string numStr) {
    try {
        return stoi(numStr);
    } catch (const std::exception& e) {
        std::cout << "Numeric value expected for " << numStr << std::endl;
        return -1;
    }
}

Macro Loader::loadMacro(std::string path, KeyboardMap& keyMap) {
	std::ifstream macroFile(path);
    Macro result(path.substr(path.find_last_of("/") + 1, path.find_last_of(".")));
    if (macroFile.is_open()) {
        std::string line;
        while (std::getline(macroFile, line)) {
            if (line.front() == '#') continue;
            std::string tok;
            std::istringstream ss(line);
            std::vector<std::string> tokens;

            while (ss >> tok) tokens.push_back(tok);
            if (tokens.size() != 3) {
                std::cout << "Skipping malformed macro line!" << std::endl;
                std::cout << line << std::endl;
                continue;
            }

            unsigned int uapiInputEventKeyCode = keyMap.getKeyValue(tokens[0]);
            std::cout << "STR: " << tokens[0] << ", UIEK: " << uapiInputEventKeyCode << std::endl;

            result.addResponse(
                tokens[0], 
                safeStrToNum(tokens[1]), 
                safeStrToNum(tokens[2]) 
            );
        }
    }
    macroFile.close();
    return result;
}

void Loader::load(DeviceManager& manager, KeyboardMap& keyMap) {
    std::string macrosPath = disk->rootDir() + "/macros";
    std::string configPath = disk->rootDir() + "/synaptix.conf";
	std::ifstream configFile(configPath);
    seteuid(0);

    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            if (line.front() == '#') continue;
            std::string tok;
            std::istringstream ss(line);
            std::vector<std::string> tokens;

            while (ss >> tok) tokens.push_back(tok);
            if (tokens.size() != 5) {
                std::cout << "Skipping malformed configuration line!" << std::endl;
                std::cout << line << std::endl;
                continue;
            }

            Macro macro = loadMacro(macrosPath + "/" + tokens[0], keyMap);
            macro.setActivator({
                safeStrToNum(tokens[2]), 
                safeStrToNum(tokens[3]), 
                ActivatorType(safeStrToNum(tokens[4]))
            });
            manager.bindMacro(tokens[1], macro);
        }
        configFile.close();
    } else {
        std::cout << "File failed to open!" << std::endl;
    }
}
