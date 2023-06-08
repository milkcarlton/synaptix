#include <chrono>
#include <filesystem>
#include <vector>
#include "device.h"
#include "keyboard.h"

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;

MacroDevice::MacroDevice(std::string path) {
    if (!std::filesystem::exists(path))
        throw std::invalid_argument("Device @ [\'" + path + "\'] not found!");
    this->devicePath = path;
    this->macros = std::vector<Macro>();
	this->deviceStream = openDeviceStream();
    this->governorThread = NULL;
}

MacroDevice::~MacroDevice() {
    if (running) {
        this->toggleMacros(false);
        governorThread->join();
        delete governorThread;
    } 

	deviceStream->close();
	delete deviceStream;
}

void MacroDevice::registerMacro(Macro macro) {
    macros.push_back(macro);
}

input_event MacroDevice::readDevice() {
	return readDevice(this->deviceStream);
}

input_event MacroDevice::readDevice(std::ifstream* stream) {
	input_event event;
	stream->read(reinterpret_cast<char*>(&event), sizeof(event));
	return event;
}

std::ifstream* MacroDevice::openDeviceStream() {
	std::ifstream* input_file = new std::ifstream();
	std::cout << "# Opening Device [\'" << devicePath << "\']" << std::endl;
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
		if (result.type == typeFilter || typeFilter == -1) 
			if (result.value == keyState || keyState == -1)
				break;
	}	
	return result;
}

void MacroDevice::recordMacro(KeyboardMap& kbm, int typeFilter, std::string outputPath) {
	auto result = std::vector<PlaybackBind>();
	std::cout << "# Recording on [ " << devicePath << " ] ..." << std::endl;
	std::cout << "# Enter key to signal the end of the recording:" << std::endl;
	input_event stopKey = pollUntilReceived(typeFilter, 1);
    std::string stopKeyAlias = kbm.getKeyAlias(stopKey.code);

	pollUntilReceived(typeFilter, 0);
	std::cout << "# Recording input until the \'" << stopKeyAlias << "\' (" << stopKey.code << ")"
			  << " key is pressed." << std::endl;

	milliseconds timePrev = milliseconds(0);
	milliseconds timeCur = milliseconds(0);

	while (true) {
		input_event e = readDevice();

		if (e.code == stopKey.code && e.type == stopKey.type) {
			if (result.size() > 0 && e.value > 0)
                break;
			else 
                continue;
        }

		if (e.type == typeFilter || typeFilter == -1) {
			timeCur = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
			);
			
			auto bind = e.code;
			auto delay = static_cast<unsigned short>((timeCur - timePrev).count());
			bool state = (e.value > 0);
			
			if (timePrev == milliseconds(0)) timePrev = timeCur;

			if (result.size() > 0) {
				PlaybackBind& b = result.back();
				if (bind == b.bind && state == b.state) 
					continue;
				b.delay = delay;
			}

			result.push_back({ bind, state, 0 });
			timePrev = timeCur;
		}
	}

	std::cout << std::endl;

	bool outToFile = (outputPath != "");
	std::ofstream outStream;
	
	if (outToFile) {
   		if (std::filesystem::exists(outputPath))
      		throw std::invalid_argument("File @ [\'" + outputPath + "\'] already exists!");
		outStream.open(outputPath, std::ios::out | std::ios::trunc);
	}

	for (size_t i = 0; i < result.size(); i++) {
		const PlaybackBind& b = result.at(i);
        std::string keyAlias = kbm.getKeyAlias(b.bind);
        if (keyAlias == "Not Found") continue;

		std::cout << keyAlias << "\t" << b.state << "\t" << b.delay << std::endl;
		if (outToFile)
			outStream << keyAlias << "\t" << b.state << "\t" << b.delay << std::endl;
	}

	if (outToFile) outStream.close();
}

void MacroDevice::startMacros() {
	toggleMacros(true);
    this->governorThread = new std::thread([this]() {
        governMacros();
    });
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
