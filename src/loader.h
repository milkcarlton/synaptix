#ifndef LOADER_H
#define LOADER_H

#include <cstdio>
#include <string>
#include "device.h"
#include "device_manager.h"

class Loader {
	public:
		Loader();
		~Loader();
		
		std::string rootDir();
		Macro loadMacro(std::string path);
		void load(DeviceManager& manager);
	private:
		std::string rootDirectory = "";
};

#endif
