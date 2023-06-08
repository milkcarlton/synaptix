#include <thread>
#include "macro.h"

Macro::Macro(const std::string& name, Input* input, ActivatorBind activator) {
	this->macroName = name;
    this->input = input;
	this->responseSequence = std::vector<PlaybackBind>();
    setActivator(activator);
}

Macro::Macro(const std::string& name, Input* input) : Macro(name, input, { 0, 0, NONE }) { }

Macro::~Macro() {
	
}

void Macro::setActivator(ActivatorBind activator) {
    this->activatorBinding = activator;
}

bool Macro::isActivator(unsigned short type, unsigned short keycode, ActivatorType value) {
	return (activatorBinding.eventType == type && activatorBinding.keyBinding == keycode && 
		   (activatorBinding.responseType == value || activatorBinding.responseType == HELD));
}

void Macro::addResponse(unsigned int bind, unsigned short state, unsigned short delay) {
    struct PlaybackBind b = { bind, state, delay };
    this->responseSequence.push_back(PlaybackBind(b));
}

void Macro::playMacro() {
    for (PlaybackBind b : this->responseSequence) {
        input->playBind(b.bind, b.state, b.delay);
    }
}

void Macro::repeatMacro() {
    while (doRepeat)
		playMacro();
}

void Macro::playMacro(ActivatorType value) {
    if (activatorBinding.responseType == HELD) {
        doRepeat = (value > 0); 
        if (value == PRESS) {
            std::thread repeatThread([this]() {
                repeatMacro();
            });
            repeatThread.detach();
        }
    } else if (value == activatorBinding.responseType) {
		playMacro();
    } 
}
