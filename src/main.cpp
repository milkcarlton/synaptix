#include <bits/chrono.h>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include "device.h"
#include "device_manager.h"
#include "disk.h"
#include "keyboard.h"
#include "loader.h"

using std::string, std::cout, std::endl;

char getSafeChar(std::string str, size_t pos) {
    if (pos < str.size()) return str.at(pos);
	throw std::out_of_range("Invalid character position.");
}

int strToInt(std::string str, int defaultValue) {
	int result;
	try {
       	result = std::stoi(str);
   	} catch (const std::invalid_argument& e) {
		result = defaultValue;
   	}
	return result;
}

void signalHandler(int val) {
	(void)val;

    signal(SIGINT, signalHandler);
	exit(0);
}

void loadConfig(DiskUtils* disk, KeyboardMap& keyMap) {
    DeviceManager dm;
    Loader loader(disk);
	loader.load(dm, keyMap);

    signal(SIGINT, signalHandler);

	while (dm.hasDevices())
		std::this_thread::sleep_for (std::chrono::seconds(1));
}

void printImproper() {
    std::cout << "Improper command usage. Run with -h for help!" << std::endl;
}

void printHelp(const std::string& helpFor) {
	if (helpFor == "" || helpFor == "h") {
		std::cout << "usage: synaptix <operation> [...]" << std::endl;
		std::cout << "operations:" << std::endl;
		std::cout << "  synaptix {-h}   [modifiers] - Display this text and exit" << std::endl;
    	std::cout << "  synaptix {-l}   [modifiers] - Load and run config file" << std::endl;
    	std::cout << "  synaptix {-i}   [modifiers] - Inspect input device" << std::endl;
    	std::cout << "  synaptix {-r}   [modifiers] - Record macro" << std::endl;
    	std::cout << "  synaptix {-n}   [modifiers] - Get input device name" << std::endl;
    	std::cout << "  synaptix {-ls}  [modifiers] - Show device names" << std::endl;
		std::cout << std::endl;
    	std::cout << "run \'synaptix -h\' followed by an operation for additional information" << std::endl;
    	std::cout << "\tex: \'synaptix -h r\'" << std::endl;
	} else if (helpFor == "l") {
		std::cout << "usage: synaptix {-l} (optional config path)" << std::endl;
	} else if (helpFor == "i") {
		std::cout << "usage: synaptix {-i} (optional input filter) [modifiers]" << std::endl;
		std::cout << "modifiers:" << std::endl;
		std::cout << "  -d <path> (required)\tEvent path of desired input device" << std::endl;
	} else if (helpFor == "r") {
		std::cout << "usage: synaptix {-r} (optional input filter) [modifiers]" << std::endl;
		std::cout << "modifiers:" << std::endl;
		std::cout << "  -d <path> (required)\tDesired keyboard event device path" << std::endl;
		std::cout << "  -o <path> (optional)\tOutput path for recorded macro" << std::endl;
	} else if (helpFor == "n") {
		std::cout << "usage: synaptix {-n} [modifiers]" << std::endl;
		std::cout << "modifiers:" << std::endl;
		std::cout << "  -d <path> (required)\tDesired event path" << std::endl;
	} else if (helpFor == "ls") {
		std::cout << "usage: synaptix {-ls} [modifiers]" << std::endl;
		std::cout << "modifiers:" << std::endl;
		std::cout << "  -d <path> (optional)\tPath to directory of events" << std::endl;
	}
}

void recordMacro(const std::string& devicePath, const std::string& outputPath, int typeFilter, KeyboardMap& kb) {
    MacroDevice mdev = MacroDevice(devicePath);
	mdev.recordMacro(kb, typeFilter, outputPath);
}

void monitorDevice(const std::string& devicePath, int typeFilter) {
	std::cout << "Monitoring Device Input..." << std::endl;
    MacroDevice mdev = MacroDevice(devicePath);
    mdev.inspectDevice(typeFilter);
}

// TODO: Implement functionality
void findDevice(const std::string& rootDir, int typeFilter) {
    DeviceManager dm;
	//dm.attachDevice("/dev/input/event3", false);
	std::string devicePath = dm.findNextDevice(rootDir, typeFilter);
	std::cout << devicePath << std::endl;
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
	DiskUtils* configRoot;
	
	if (inputMap->count("-c")) {
		configRoot = new DiskUtils(inputMap->at("-c"));
	} else {
		configRoot = new DiskUtils();
	}

	KeyboardMap keyMap(configRoot);

	if (inputMap->count("-l")) {
		loadConfig(configRoot, keyMap);
	} else if (inputMap->count("-h")) {
		printHelp(inputMap->at("-h"));
	} else if (inputMap->count("-i")) {
		int typeFilter = strToInt(inputMap->at("-i"), -1);
		if (inputMap->count("-d")) monitorDevice(inputMap->at("-d"), typeFilter);
		else printImproper();
	} else if (inputMap->count("-r")) {
		int typeFilter = strToInt(inputMap->at("-r"), 1);
		std::string outputPath = (inputMap->count("-o")) ? inputMap->at("-o") : "";
		if (inputMap->count("-d")) recordMacro(inputMap->at("-d"), outputPath, typeFilter, keyMap);
		else printImproper();
	} else if (inputMap->count("-f")) {
		std::string rootDir = "/dev/input/";
		if (inputMap->count("-d")) rootDir = inputMap->at("-d");
		int typeFilter = strToInt(inputMap->at("-f"), 1);
		findDevice(rootDir, typeFilter);	
	} else if (inputMap->count("-g")) {
		keyMap.genKeyMapFromSrcDefs(true);
	} else if (inputMap->count("-t")) {
		printImproper();
	} else if (inputMap->count("-n")) {
		if (inputMap->count("-d"))
			MacroDevice::printDeviceInfo(inputMap->at("-d"), true);
		else printImproper();
	} else if (inputMap->count("-ls")) {
		std::string rootDir = "/dev/input/";
		if (inputMap->count("-d")) rootDir = inputMap->at("-d");
		for (const auto& entry : std::filesystem::directory_iterator(rootDir))
			if (std::filesystem::is_character_file(entry))
				MacroDevice::printDeviceInfo(entry.path());
	} else if (inputMap->count("-b")) {
		std::cout << "TODO: Implement one-time bind testing" << std::endl;
	} else {
		printImproper();
	}

	delete configRoot;
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
