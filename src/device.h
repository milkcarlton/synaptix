#ifndef DEVICE_H
#define DEVICE_H

#include "macro.h"
#include <linux/input.h>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
class KeyboardMap;

class MacroDevice {
	public:
		explicit MacroDevice(std::string path);
		~MacroDevice();
		
		input_event readDevice();
		input_event readDevice(std::ifstream* stream);
		std::ifstream* openDeviceStream();
		void registerMacro(Macro macro);
		void startMacros();
		void governMacros();
		
		void inspectDevice(int typeFilter);
		void recordMacro(KeyboardMap& kbm, int typeFilter, std::string outputPath);

		bool hasMatchingEvent(int typeFilter, int keyState);
		input_event pollUntilReceived(int typeFilter, int keyState);
		
		bool isRunning();
		bool toggleMacros();
		bool toggleMacros(bool toggle);
		const std::string getDevicePath() { return devicePath; }

		bool operator==(const MacroDevice& other) {
			return this->devicePath == other.devicePath;
		}
		
		static std::string getDeviceName(const std::string& path);
		static void printDeviceInfo(const std::string& path, bool extended = false);

	private:
		bool running = false;
		std::string devicePath;
		std::vector<Macro> macros;
		std::ifstream* deviceStream;
		std::thread* governorThread;
};

#endif
