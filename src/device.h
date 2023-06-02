#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <linux/input.h>
#include <linux/input-event-codes.h>

#include "macro.h"

class MacroDevice {
	public:
		MacroDevice(std::string path);
		~MacroDevice();
		
		input_event readDevice();
		input_event readDevice(std::ifstream* stream);
		std::ifstream* openDeviceStream();
		void registerMacro(Macro macro);
		void unregisterMacro(Macro macro);
		void startMacros();
		void governMacros();
		
		void inspectDevice(int typeFilter);
		void recordMacro(int typeFilter, std::string outputPath);

		bool hasMatchingEvent(int typeFilter, int keyState);
		input_event pollUntilReceived(int typeFilter, int keyState);
		
		bool isRunning();
		bool toggleMacros();
		bool toggleMacros(bool toggle);
		const std::string getDevicePath() { return devicePath; }

		bool operator==(const MacroDevice& other) {
    		return this->devicePath == other.devicePath;
		}
	private:
		bool running = false;
		std::string devicePath;
		std::vector<Macro> macros;
		std::ifstream* deviceStream;
        std::thread* governorThread;
};

#endif
