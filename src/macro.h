#ifndef MACRO_H
#define MACRO_H

#include <string>
#include <vector>
#include <iostream>

#include "keyboard.h"
#include "input.h"

enum ActivatorType { RELEASE, PRESS, HELD, NONE };

struct ActivatorBind {
    unsigned short eventType;
    unsigned short keyBinding;
    ActivatorType responseType;
};

struct PlaybackBind {
	unsigned int bind;
	unsigned short state;
	unsigned short delay;
};

class Macro {
    public:
        Macro(const std::string& name, Input* input, ActivatorBind activator);
        Macro(const std::string& name, Input* input);
        ~Macro();
        void playMacro(ActivatorType value);
        bool isActivator(unsigned short type, unsigned short keycode, ActivatorType value);
        void addResponse(unsigned int bind, unsigned short state, unsigned short delay);
        void setActivator(ActivatorBind activator);

    private:
        void repeatMacro();
        void playMacro();
        std::string macroName;
        ActivatorBind activatorBinding;
        std::vector<PlaybackBind> responseSequence;
        Input* input;
        bool doRepeat = false;
};

#endif
