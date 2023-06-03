#ifndef AGNOSTIC_INPUT_H
#define AGNOSTIC_INPUT_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <linux/input.h>
#include <linux/input-event-codes.h>

class Input {
	public:
		Input(std::string path = "/dev/uinput");
		~Input();
       	
		unsigned int getSymToKeycode(unsigned int keysym);
       	unsigned int getStrToKeycode(std::string keyStr);
		
		void emitKeycode(unsigned int keycode, unsigned short state);
		void emitEvent(unsigned short type, unsigned int code, unsigned short value);

		void playBind(unsigned int keycode);
		void playBind(unsigned int keycode, unsigned short state, unsigned short delay = 0);

	private:
		int fildes;
};

#endif
