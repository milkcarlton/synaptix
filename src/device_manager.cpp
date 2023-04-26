#include <thread>
#include "device_manager.h"

DeviceManager::DeviceManager() {
    devices = std::vector<MacroDevice*>(); 
}

DeviceManager::~DeviceManager() {
    for (MacroDevice* md : devices)
        delete md;
}

void DeviceManager::showDevices() {
    std::cout << "Devices:" << std::endl;
    for (MacroDevice* md : devices)
        std::cout << "\t- " << md->getDevicePath() << std::endl;
}

MacroDevice* DeviceManager::getDevice(std::string devicePath) {
    return devices.at(getDeviceIndex(devicePath));
}

bool DeviceManager::hasDevice(std::string devicePath) {
    return getDeviceIndex(devicePath) != -1;
}

int DeviceManager::getDeviceIndex(std::string devicePath) {
    for (int i = 0; i < devices.size(); i++)
        if (devicePath == devices.at(i)->getDevicePath()) 
            return i;
    return -1;
}

int DeviceManager::attachDevice(std::string devicePath) {
    int deviceIndex = getDeviceIndex(devicePath);
    if (deviceIndex == -1) {
        MacroDevice* device = new MacroDevice(devicePath);
        devices.push_back(device);
        deviceIndex = devices.size() - 1;
        device->startMacros();
    }
    return deviceIndex;
}

void DeviceManager::bindMacro(std::string devicePath, Macro macro) {
    MacroDevice* device = devices.at(attachDevice(devicePath));
    device->registerMacro(macro);
}
