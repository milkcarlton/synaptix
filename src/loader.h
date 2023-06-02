#ifndef LOADER_H
#define LOADER_H

#include <cstdio>
#include <string>

#include "disk.h"
#include "device.h"
#include "keyboard.h"
#include "device_manager.h"

class Loader {
	public:
        Loader(DiskUtils* disk);
		~Loader();
		
		Macro loadMacro(std::string path, KeyboardMap& keyMap);
		void load(DeviceManager& manager, KeyboardMap& keyMap);
	private:
		DiskUtils* disk;
};

#endif
