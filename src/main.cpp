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

void printHelp(std::string helpFor) {
	if (helpFor == "" || helpFor == "h") {
		std::cout << "usage: synaptix <operation> [...]" << std::endl;
		std::cout << "operations:" << std::endl;
		std::cout << "  synaptix {-h}\t[options] - Display this help and exit" << std::endl;
    	std::cout << "  synaptix {-l}\t[options] - Load and run config file" << std::endl;
    	std::cout << "  synaptix {-i}\t[options] - Inspect input device" << std::endl;
    	std::cout << "  synaptix {-r}\t[options] - Record macro" << std::endl;
		std::cout << std::endl;
    	std::cout << "run \'synaptix -h\' with an operation for further help" << std::endl;
    	std::cout << "\tex: \'synaptix -h r\'" << std::endl;
	} else if (helpFor == "l") {
		std::cout << "usage: synaptix {-l} (optional config path)" << std::endl;
		std::cout << "options:" << std::endl;
	} else if (helpFor == "i") {
		std::cout << "usage: synaptix {-i} (optional input filter) [options]" << std::endl;
		std::cout << "options:" << std::endl;
		std::cout << "  -d <path> (required)\tEvent path of desired input device" << std::endl;
	} else if (helpFor == "r") {
		std::cout << "usage: synaptix {-r} (optional input filter) [options]" << std::endl;
		std::cout << "options:" << std::endl;
		std::cout << "  -d <path> (required)\tDesired keyboard event device path" << std::endl;
		std::cout << "  -o <path> (optional)\tOutput path for recorded macro" << std::endl;
	}
}

void recordMacro(std::string devicePath, std::string outputPath, int typeFilter) {
    MacroDevice mdev = MacroDevice(devicePath);
	mdev.recordMacro(typeFilter, outputPath);
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
		printHelp(inputMap->at("-h"));
	} else if (inputMap->count("-i")) {
		int typeFilter = strToInt(inputMap->at("-i"), -1);
		if (inputMap->count("-d")) monitorDevice(inputMap->at("-d"), typeFilter);
		else printImproper();
	} else if (inputMap->count("-r")) {
		int typeFilter = strToInt(inputMap->at("-r"), 1);
		std::string outputPath = (inputMap->count("-o")) ? inputMap->at("-o") : "";
		if (inputMap->count("-d")) recordMacro(inputMap->at("-d"), outputPath, typeFilter);
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
