#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/capability.h>

#include "macro.h"
#include "device.h"
#include "loader.h"
#include "device_manager.h"

using std::string, std::cout, std::endl;

char getSafeChar(std::string str, short pos) {
    if (pos >= 0 && pos < str.size()) return str.at(pos);
	throw std::out_of_range("Invalid character position.");
}

int strToInt(std::string str, int defaultValue) {
	int result;
	try {
       	result = std::stoi(str);
   	} catch (std::invalid_argument& e) {
		result = defaultValue;
   	}
	return result;
}

void loadConfig(std::string rootDir = "") {
    DeviceManager dm;
    Loader loader(rootDir);

	loader.load(dm);
	while (true) {}
}

void printImproper() {
    std::cout << "Improper command usage. Run with -h for help!" << std::endl;
}

void printHelp() {
	std::cout << "usage: synaptix <operation> [...]" << std::endl;
	std::cout << "  {-h}\tDisplay this help and exit" << std::endl;
    std::cout << "  {-l}\t[optional path]: Load and run config" << std::endl;
    std::cout << "  {-d}\t[path]: Specify device" << std::endl;
    std::cout << "  \t{-i}\t[optional input filter]: Inspect input device" << std::endl;
    std::cout << "  \t{-r}\tRecord macro" << std::endl;
}

void recordMacro(std::string devicePath, int typeFilter) {
	std::cout << "Recording Macro..." << std::endl;
    MacroDevice mdev = MacroDevice(devicePath);
	mdev.recordMacro(typeFilter);
}

void monitorDevice(std::string devicePath, int typeFilter) {
	std::cout << "Monitoring Device Input..." << std::endl;
    MacroDevice mdev = MacroDevice(devicePath);
    mdev.inspectDevice(typeFilter);
}

std::unordered_map<std::string, std::string>* mapInput(int argc, char** argv) {
	auto result = new std::unordered_map<std::string, std::string>();
    for (int i = 1; i < argc; i++) {
        std::string argStr = argv[i];
       	 
		try {
    		char fchar = getSafeChar(argStr, 0);
			if (fchar == '-') {
				if ((i + 1) < argc) {
        			std::string nextArg = argv[i + 1];
					if (getSafeChar(nextArg, 0) != '-') {
						result->insert(std::pair<std::string, std::string>(argStr, nextArg));
						i++;
						continue;
					}
				} 
				result->insert(std::pair<std::string, std::string>(argStr, ""));
			}
		} catch (std::out_of_range& e) {
			throw std::out_of_range("Invalid program arguments.");
		}
    }
	return result;
}

void parseArguments(std::unordered_map<std::string, std::string>* inputMap) {
	if (inputMap->count("-l")) {
		loadConfig(inputMap->at("-l"));
	} else if (inputMap->count("-h")) {
		printHelp();
	} else if (inputMap->count("-i")) {
		int typeFilter = strToInt(inputMap->at("-i"), -1);
		if (inputMap->count("-d")) monitorDevice(inputMap->at("-d"), typeFilter);
		else printImproper();
	} else if (inputMap->count("-r")) {
		int typeFilter = strToInt(inputMap->at("-r"), 1);
		if (inputMap->count("-d")) recordMacro(inputMap->at("-d"), typeFilter);
		else printImproper();
	} else if (inputMap->count("-t")) {
    // Use for one-time binding / testing
	} else {
		printImproper();
	}
}

int main(int argc, char** argv) {
	try {
    	seteuid(0);
		auto inputMap = mapInput(argc, argv);
		parseArguments(inputMap);
		delete inputMap;
	} catch (std::exception& e) {
		std::cerr << e.what() << endl;
	}

	return 0;
}
