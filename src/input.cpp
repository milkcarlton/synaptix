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

void Input::playBind(unsigned int keycode, unsigned short delay, unsigned short time_held) {
    XTestFakeKeyEvent(display, keycode, True, 0);
    //time held
    XTestFakeKeyEvent(display, keycode, False, 0);
    XFlush(display);
    //delay
}
