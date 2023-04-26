#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <vector>
#include <string>
#include "device.h"
#include "macro.h"

// Change to references&reference wrappers / pointers
class DeviceManager {
    public:
        DeviceManager();
        ~DeviceManager();
        
		bool hasDevice(std::string devicePath);
		MacroDevice* getDevice(std::string devicePath);
		int getDeviceIndex(std::string devicePath);
        int attachDevice(std::string devicePath);
		void bindMacro(std::string devicePath, Macro macro);
        void showDevices();
    private:
        std::vector<MacroDevice*> devices;
};

#endif
