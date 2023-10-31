#include "input.h"
#include <bits/chrono.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <thread>

Input::Input(const std::string& path) {
	this->fildes = open(path.c_str(), O_WRONLY | O_NONBLOCK);
	if (this->fildes == -1) {
        throw std::invalid_argument(
            "Error opening uinput [" + path + "]: " + std::string(strerror(errno))
        );
	}

	ioctl(fildes, UI_SET_EVBIT, EV_KEY);

    for (int i = 0; i < KEY_CNT; i++) {
        ioctl(fildes, UI_SET_KEYBIT, i);
    }

    for (int i = BTN_MISC; i <= BTN_GEAR_UP; i++) {
        ioctl(fildes, UI_SET_KEYBIT, i);
    }

	struct uinput_setup setup;

	memset(&setup, 0, sizeof(setup));
	setup.id.bustype = BUS_USB;
	setup.id.vendor = 0x0734;
	setup.id.product = 0x0078;
	strcpy(setup.name, "Virtual Input");
	
	ioctl(fildes, UI_DEV_SETUP, &setup);
	ioctl(fildes, UI_DEV_CREATE);
	
	sleep(1);
}

Input::~Input() {
	sleep(1);
	ioctl(this->fildes, UI_DEV_DESTROY);
	close(this->fildes);
}

void Input::emitKeycode(unsigned int keycode, unsigned short state) {
    emitEvent(EV_KEY, keycode, state);
	emitEvent(EV_SYN, SYN_REPORT, 0); 
}

void Input::emitEvent(unsigned short type, unsigned int code, unsigned short value) {
	struct input_event e;
	e.type = type;	
	e.code = code;
	e.value = value;	
	e.time.tv_sec = 0;
	e.time.tv_usec = 0;

	write(this->fildes, &e, sizeof(e));
}

void Input::playBind(unsigned int keycode) {
    playBind(keycode, 0, 0);
}

void Input::playBind(unsigned int keycode, unsigned short state, unsigned short delay) {
	if (state == 2) {
		emitKeycode(keycode, true);	
		emitKeycode(keycode, false);
	} else if (state == 3) {
		emitKeycode(keycode, state);	
	} else {
		if (state > 0) state = 1;
		emitKeycode(keycode, state);	
	}
    if (delay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}
