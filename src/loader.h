#ifndef LOADER_H
#define LOADER_H

#include <string>
#include "macro.h"
class DeviceManager;
class DiskUtils;
class Input;
class KeyboardMap;

class Loader {
	public:
		explicit Loader(DiskUtils* disk);
		~Loader();
		
		Macro loadMacro(std::string path, KeyboardMap& keyMap);
		void load(DeviceManager& manager, KeyboardMap& keyMap);

	private:
		Loader(const Loader&) = delete;
		Loader& operator=(const Loader&);

		DiskUtils* disk;
		Input* input;
};

#endif
