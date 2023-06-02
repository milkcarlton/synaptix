#include <chrono>
#include <thread>
#include "input_xorg.h"

InputXorg::InputXorg() {
    this->display = XOpenDisplay(NULL);
}

InputXorg::~InputXorg() { }

unsigned int InputXorg::getSymToKeycode(unsigned int keysym) {
    return XKeysymToKeycode(display, keysym);
}

unsigned int InputXorg::getStrToKeycode(std::string keyStr) {
    return XKeysymToKeycode(display, XStringToKeysym(keyStr.c_str()));
}

void InputXorg::playBind(unsigned int keycode) {
    playBind(keycode, 0, 0);
}

void InputXorg::playBind(unsigned int keycode, unsigned short state, unsigned short delay) {
    if (state == 2) {
        XTestFakeKeyEvent(display, keycode, true, 0);
        XTestFakeKeyEvent(display, keycode, false, 0);
    } else {
        if (state > 0) state = 1;
        XTestFakeKeyEvent(display, keycode, state, 0);
    }
    XFlush(display);
    if (delay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}
