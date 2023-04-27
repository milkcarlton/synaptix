#include "input.h"

Input::Input() {
    this->display = XOpenDisplay(NULL);
}

Input::~Input() {

}

unsigned int Input::getSymToKeycode(unsigned int keysym) {
    return XKeysymToKeycode(display, keysym);
}

void Input::playBind(unsigned int keycode) {
    playBind(keycode, 0, 0);
}

void Input::playBind(unsigned int keycode, unsigned short state, unsigned short delay) {
    if (state == 2) {
        XTestFakeKeyEvent(display, keycode, true, 0);
        XTestFakeKeyEvent(display, keycode, false, 0);
    } else {
        if (state > 0) state = 1;
        XTestFakeKeyEvent(display, keycode, state, 0);
    }
    XFlush(display);
    //if (delay > 0) sleep(delay);
}
