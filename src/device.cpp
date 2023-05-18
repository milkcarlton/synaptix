#include <thread>
#include <chrono>
#include <filesystem>
#include <vector>
#include "device.h"

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

MacroDevice::MacroDevice(std::string path) {
    if (!std::filesystem::exists(path))
        throw std::invalid_argument("Device @ [\'" + path + "\'] not found!");
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
	std::cout << "Opening Device [\'" << devicePath << "\']" << std::endl;
	input_file->open(devicePath, std::ios::binary | std::ios::in);
	return input_file;
}

void MacroDevice::governMacros() {
    while (running) {
		input_event e = readDevice();
        for (Macro& m : macros) {
            if (m.isActivator(e.type, e.code, ActivatorType(e.value)))
                m.playMacro(ActivatorType(e.value));
		}
    }
}

void MacroDevice::inspectDevice(int typeFilter) {
	std::cout << "Watching [\'" << devicePath << "\'] for event type: " << typeFilter << std::endl;
	while (true) {
		input_event e = readDevice();
		if (e.type == typeFilter || typeFilter == -1) {
			std::cout << "[ " << devicePath << " ]" << std::endl;
			std::cout << "\tType: " << e.type << std::endl;
			std::cout << "\tCode: " << e.code << std::endl;
			std::cout << "\tValue: " << e.value << std::endl;
		}
	}
}

input_event MacroDevice::pollUntilReceived(int typeFilter, int keyState) {
	input_event result;
	while (true) {
		result = readDevice();
		if (result.type == typeFilter || typeFilter == -1) break;
	}	
	return result;
}

void MacroDevice::recordMacro(int typeFilter) {
	auto result = std::vector<PlaybackBind>();
	std::cout << "Recording on [ " << devicePath << " ] ..." << std::endl;
	std::cout << "Enter key to signal the end of the recording..." << std::endl;
	input_event stopKey = pollUntilReceived(1, 1);
	pollUntilReceived(1, 0);
	std::cout << "Recording until key " << stopKey.code << " is pressed!" << std::endl;

	milliseconds timePrev = milliseconds(0);
	milliseconds timeCur = milliseconds(0);

	while (true) {
		input_event e = readDevice();
		if (e.code == stopKey.code && e.type == stopKey.type && e.value > 0) break;
		if (e.type == typeFilter || typeFilter == -1) {
			milliseconds timeCur = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
			);
			
			if (timePrev == milliseconds(0)) timePrev = timeCur;
			if (result.size() > 0 && e.value == result.back().state) continue;

			std::string bind = "A";
			unsigned short state = static_cast<unsigned short>(e.value);
			unsigned short delay = static_cast<unsigned short>((timeCur - timePrev).count());

			std::cout << e.code << "\tV: " << state << "\tD: " << delay << std::endl;
			struct PlaybackBind eventBind = { bind, state, delay };
			
			result.push_back(eventBind);
			timePrev = timeCur;	
		}
	}

	for (int i = 0; i < result.size(); i++) {
		PlaybackBind b = result.at(i);
		std::cout << b.bind << " -> VAL: " << b.state << " DELAY: " << b.delay << std::endl;
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
