#ifndef INPUT_H
#define INPUT_H

#include <X11/extensions/XTest.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

// TODO: Turn into abstract w/ implementations for xorg and wayland
class Input {
    public:
       Input();
       ~Input();
       unsigned int getSymToKeycode(unsigned int keysym);
       unsigned int getStrToKeycode(std::string keyStr);
       void playBind(unsigned int keycode, unsigned short state, unsigned short delay = 0);
       void playBind(unsigned int keycode);
    private:
       std::string type;
       Display* display;
};

#endif
