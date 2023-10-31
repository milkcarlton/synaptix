#include "device_manager.h"
#include "device.h"
#include <stdio.h>
#include <iostream>
class Macro;

DeviceManager::DeviceManager() : devices(std::vector<MacroDevice*>()) { }

DeviceManager::~DeviceManager() {
    auto it = devices.begin();
    while (it != devices.end()) {
        MacroDevice* md = *it;
        md->toggleMacros(false);
        it = devices.erase(it);
        delete md;
    }
}

void DeviceManager::showDevices() {
    std::cout << "Devices:" << std::endl;
    for (MacroDevice* md : devices)
        std::cout << "\t- " << md->getDevicePath() << std::endl;
}

MacroDevice* DeviceManager::getDevice(const std::string& devicePath) {
    return devices.at(getDeviceIndex(devicePath));
}

bool DeviceManager::hasDevice(const std::string& devicePath) {
    return getDeviceIndex(devicePath) != -1;
}

bool DeviceManager::hasDevices() {
   return (devices.size() > 0); 
}

int DeviceManager::getDeviceIndex(const std::string& devicePath) {
    for (size_t i = 0; i < devices.size(); i++)
        if (devicePath == devices.at(i)->getDevicePath()) 
            return i;
    return -1;
}

int DeviceManager::attachDevice(const std::string& devicePath, bool startMacros) {
    int deviceIndex = getDeviceIndex(devicePath);
    if (deviceIndex == -1) {
        MacroDevice* device = new MacroDevice(devicePath);
        devices.push_back(device);
        deviceIndex = devices.size() - 1;
        if (startMacros)
            device->startMacros();
    }
    return deviceIndex;
}

void DeviceManager::bindMacro(const std::string& devicePath, const Macro& macro) {
    MacroDevice* device = devices.at(attachDevice(devicePath));
    device->registerMacro(macro);
}

std::string DeviceManager::findNextDevice(const std::string& path, unsigned short typeFilter) {
    (void)path;
    (void)typeFilter;

    return "Unimplemented";
}
