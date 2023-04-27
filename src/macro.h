#ifndef MACRO_H
#define MACRO_H

#include <string>
#include <vector>
#include <iostream>

#include "input.h"

enum ActivatorType { RELEASE, PRESS, HELD, NONE };

struct ActivatorBind {
    unsigned short eventType;
    unsigned short keyBinding;
    ActivatorType responseType;
};

struct PlaybackBind {
	unsigned short code;
	unsigned short state;
	unsigned short delay;
};

class Macro {
    public:
        Macro(std::string name, ActivatorBind activator);
        Macro(std::string name);
        ~Macro();
        void playMacro(ActivatorType value);
        //for a loop macro, accept both values
        bool isActivator(unsigned short type, unsigned short keycode, ActivatorType value);
        void addResponse(unsigned short code, unsigned short time_held, unsigned short delay);
        void setActivator(ActivatorBind activator);
    private:
        void repeatMacro();
        std::string macroName; // file name of macro in config folder
        ActivatorBind activatorBinding;
        std::vector<PlaybackBind> responseSequence;
        Input input;
        bool doRepeat = false;
};

#endif
