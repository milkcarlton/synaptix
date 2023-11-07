#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <vector>
#include <string>
class Macro;
class MacroDevice;

class DeviceManager {
	public:
		DeviceManager();
		~DeviceManager();
		
		bool hasDevice(const std::string& devicePath);
		bool hasDevices();
		MacroDevice* getDevice(const std::string& devicePath);
		int getDeviceIndex(const std::string& devicePath);
		int attachDevice(const std::string& devicePath, bool startMacros = true);
		void bindMacro(const std::string& devicePath, const Macro& macro);
		void showDevices();
	
		std::string findNextDevice(const std::string& devicePath, unsigned short typeFilter);

	private:
		std::vector<MacroDevice*> devices;
};

#endif
