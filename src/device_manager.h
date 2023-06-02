#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <vector>
#include <string>
#include "device.h"
#include "macro.h"

class DeviceManager {
    public:
        DeviceManager();
        ~DeviceManager();
        
		bool hasDevice(std::string devicePath);
		bool hasDevices();
		MacroDevice* getDevice(std::string devicePath);
		int getDeviceIndex(std::string devicePath);
        int attachDevice(std::string devicePath, bool startMacros = true);
		void bindMacro(std::string devicePath, Macro macro);
        void showDevices();
    
        std::string findNextDevice(const std::string& devicePath, unsigned short typeFilter);
    private:
        std::vector<MacroDevice*> devices;
};

#endif
