/**
 Read file from ~/.config/synaptix/...
 Of format /dev/input/..., 1 [type from event], 2|1|0 [while held|on press|on release]
 Then the keycodes to execute on that binding...
 {keycode, press for ms}
 {PAUSE, pause for ms}

 Move input device mapping to separate class with array of macros
 So the input device is only polled once per macro set
 Also check if the keycode is already bound
**/

#include <thread>
#include "macro.h"

Macro::Macro(std::string name, unsigned short type, unsigned short keycode, ActivatorType value) {
	this->macroName = name;
	this->eventType = type;
	this->keyBinding = keycode;
	this->responseType = value;

    this->input = Input();
	this->responseSequence = std::vector<PlaybackBind>();
}

Macro::~Macro() {
	
}

bool Macro::isActivator(unsigned short type, unsigned short keycode, ActivatorType value) {
	return (eventType == type && keyBinding == keycode && 
		   (responseType == value || responseType == HELD));
}

void Macro::addResponse(unsigned short code, unsigned short time_held, unsigned short delay) {
    struct PlaybackBind b = { code, time_held, delay };
    this->responseSequence.push_back(PlaybackBind(b));
}

void Macro::repeatMacro() {
    while (doRepeat) 
        for (PlaybackBind b : responseSequence)
            input.playBind(input.getSymToKeycode(b.code), b.delay, b.time_held); 
}

void Macro::playMacro(ActivatorType value) {
    if (responseType == HELD) {
        doRepeat = (value == 1); 
        if (value == PRESS) {
            std::thread repeatThread([this]() {
                repeatMacro();
            });
            repeatThread.detach();
        }
    } else if (value == responseType) {
        for (PlaybackBind b : responseSequence) {
            input.playBind(input.getSymToKeycode(b.code), b.delay, b.time_held); 
        }
    } 
}
