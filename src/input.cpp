#include <chrono>
#include <thread>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>

#include "input.h"

AgnosticInput::AgnosticInput(std::string path) {
	this->fildes = open(path.c_str(), O_WRONLY | O_NONBLOCK);
	if (this->fildes == -1) {
        throw std::invalid_argument(
            "Error opening uinput [" + path + "]: " + std::string(strerror(errno))
        );
	}

	ioctl(fildes, UI_SET_EVBIT, EV_KEY);
	ioctl(fildes, UI_SET_KEYBIT, KEY_SPACE);

	struct uinput_setup setup;

	memset(&setup, 0, sizeof(setup));
	setup.id.bustype = BUS_USB;
	setup.id.vendor = 0x0734;
	setup.id.product = 0x0078;
	strcpy(setup.name, "Synaptix Input");
	
	ioctl(fildes, UI_DEV_SETUP, &setup);
	ioctl(fildes, UI_DEV_CREATE);
	
	sleep(1);
}

AgnosticInput::~AgnosticInput() {
	sleep(1);
	ioctl(this->fildes, UI_DEV_DESTROY);
	close(this->fildes);
}

void AgnosticInput::emitKeycode(unsigned int keycode, unsigned short state) {
    emitEvent(EV_KEY, KEY_SPACE, state);
	emitEvent(EV_SYN, SYN_REPORT, 0); 
}

void AgnosticInput::emitEvent(unsigned short type, unsigned int code, unsigned short value) {
	struct input_event e;
	e.type = type;	
	e.code = code;
	e.value = value;	
	e.time.tv_sec = 0;
	e.time.tv_usec = 0;

	write(this->fildes, &e, sizeof(e));
}

void AgnosticInput::playBind(unsigned int keycode) {
    playBind(keycode, 0, 0);
}

void AgnosticInput::playBind(unsigned int keycode, unsigned short state, unsigned short delay) {
	if (state == 2) {
		emitEvent(EV_KEY, keycode, true);	
		emitEvent(EV_KEY, keycode, false);	
	} else {
		if (state > 0) state = 1;
		emitEvent(EV_KEY, keycode, state);	
	}
    if (delay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}
