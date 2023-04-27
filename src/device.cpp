#include <thread>
#include <chrono>
#include "device.h"

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

MacroDevice::MacroDevice(std::string path) {
    this->devicePath = path;
    this->macros = std::vector<Macro>();
	this->deviceStream = openDeviceStream();
}

MacroDevice::~MacroDevice() {
	deviceStream->close();
	delete deviceStream;
}

void MacroDevice::registerMacro(Macro macro) {
    macros.push_back(macro);
}

void MacroDevice::unregisterMacro(Macro macro) {

}

input_event MacroDevice::readDevice() {
	input_event event;
	deviceStream->read(reinterpret_cast<char*>(&event), sizeof(event));
	return event;
}

std::ifstream* MacroDevice::openDeviceStream() {
	std::ifstream* input_file = new std::ifstream();
	std::cout << "Opening " << devicePath << std::endl;
	input_file->open(devicePath, std::ios::binary | std::ios::in);
	return input_file;
}

void MacroDevice::governMacros() {
    while (running) {
		input_event e = readDevice();
        // Thread actions 
        for (Macro& m : macros) {
            if (m.isActivator(e.type, e.code, ActivatorType(e.value))) 
                m.playMacro(ActivatorType(e.value));
		}
    }
}

void MacroDevice::inspectDevice(unsigned short filterType) {
	while (true) {
		input_event e = readDevice();
		if (e.type == EV_KEY || EV_KEY == -1) {
			std::cout << "[ " << devicePath << " ]" << std::endl;
			std::cout << "\tType: " << e.type << std::endl;
			std::cout << "\tCode: " << e.code << std::endl;
			std::cout << "\tValue: " << e.value << std::endl;
		}
	}
}

void MacroDevice::recordMacro(unsigned short filterType) {
	std::cout << "[ " << devicePath << " ]" << std::endl;
    milliseconds time = milliseconds(0);
    milliseconds timeDiff = time;
	while (true) {
		input_event e = readDevice();
		if (e.type == EV_KEY || EV_KEY == -1) {
            milliseconds timeNow = duration_cast<milliseconds>(
                system_clock::now().time_since_epoch()
            );
            if (time != milliseconds(0)) {
                timeDiff = timeNow - time;
			    std::cout << timeDiff << std::endl;
			    std::cout << e.code << "\t" << e.value << "\t";
            } else {
                timeDiff = time;
			    std::cout << e.code << "\t" << e.value << "\t" << timeDiff << std::endl;
            }
            timeDiff = timeNow - time;
            time = timeNow;
		}
	}
}

void MacroDevice::startMacros() {
	toggleMacros(true);
    std::thread governorThread([this]() {
        governMacros();
    });
    governorThread.detach();
}

bool MacroDevice::toggleMacros() {
    return toggleMacros(!running);
}

bool MacroDevice::toggleMacros(bool toggle) {
    running = toggle;
    return running;
}

bool MacroDevice::isRunning() {
    return running;
}
