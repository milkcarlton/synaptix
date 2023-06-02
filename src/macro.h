#ifndef MACRO_H
#define MACRO_H

#include <string>
#include <vector>
#include <iostream>

#include "keyboard.h"
#include "xorg/input_xorg.h"

enum ActivatorType { RELEASE, PRESS, HELD, NONE };

struct ActivatorBind {
    unsigned short eventType;
    unsigned short keyBinding;
    ActivatorType responseType;
};

struct PlaybackBind {
	std::string bind;
	unsigned short state;
	unsigned short delay;
};

class Macro {
    public:
        Macro(std::string name, ActivatorBind activator);
        Macro(std::string name);
        ~Macro();
        void playMacro(ActivatorType value);
        bool isActivator(unsigned short type, unsigned short keycode, ActivatorType value);
        void addResponse(std::string bind, unsigned short state, unsigned short delay);
        void setActivator(ActivatorBind activator);
    private:
        void repeatMacro();
        void playMacro();
        std::string macroName;
        ActivatorBind activatorBinding;
        std::vector<PlaybackBind> responseSequence;
        InputXorg input;
        bool doRepeat = false;
};

#endif
