#include "keyboard.h"

#include <fcntl.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

KeyboardMap::KeyboardMap(DiskUtils* disk) {
    this->disk = disk;
    this->genKeyMap(disk->rootDir());
}

KeyboardMap::KeyboardMap() {
    this->file_descriptor = open("/dev/tty0", O_RDWR);
    if (file_descriptor == -1) {
        perror("Failed to open device");
        throw std::invalid_argument("Invalid device for keyboard mapping!");
    }
}

KeyboardMap::~KeyboardMap() {
    close(file_descriptor);
}

void KeyboardMap::readKeyMap(std::string path) {
    std::ifstream keyConfig(path + "/keymap.conf");
    
    if (keyConfig.is_open()) {
        std::string line;
        while (std::getline(keyConfig, line)) {
            if (line.front() == '#') continue;
            std::string tok;
            std::istringstream ss(line);
            std::vector<std::string> tokens;
            
            while (ss >> tok) tokens.push_back(tok);
            if (tokens.size() != 2) {
                std::cout << "Skipping malformed key configuration line!" << std::endl;
                std::cout << line << std::endl;
                continue;
            }
            
            std::string key = tokens[0];
            for (int i = 0; i < Text.length(); i++)
                Text[i] = tolower(Text[i]);
            std::transform(key.begin(), key.end(), key.begin(), std::tolower);
            
			keyMap.insert(std::pair<std::string, unsigned int>(
                key,
                std::stoul(tokens[1])
            ));
        }
        keyConfig.close();
    } else {
        std::cout << "Key config failed to open!" << std::endl;
    }

}

unsigned int KeyboardMap::getKeyValue(std::string keyStr) {
    if (keyMap.count(keyStr))
       return keyMap.at(keyStr); 
    
    return 0;
}

std::string KeyboardMap::getKeyStr(unsigned int keyVal) {
    return "";
}

std::string KeyboardMap::getXKey(unsigned short keyCode) {
    struct kbentry entry;
    entry.kb_table = K_NORMTAB;
    entry.kb_index = keyCode;
    
    if (ioctl(file_descriptor, KDGKBENT, &entry)) {
	    perror("ioctl KDGKBENT failed");
        throw std::invalid_argument("Invalid device for keyboard mapping!");
    }

    if (entry.kb_value == K_SHIFT)
        return "Shift_L";
    else if (entry.kb_value == K_ENTER)
        return "Return";
    else if (entry.kb_value == K_CTRL)
        return "Control_L";
    else if (entry.kb_value == K_CAPS)
        return "Caps_Lock";
    else if (entry.kb_value == K_ALT)
        return "Alt_L";
    else if (entry.kb_value == K_F1)
        return "F1";
    else if (entry.kb_value == K_F2)
        return "F2";
    else if (entry.kb_value == K_F3)
        return "F3";
    else if (entry.kb_value == K_F4)
        return "F4";
    else if (entry.kb_value == K_F5)
        return "F5";
    else if (entry.kb_value == K_F6)
        return "F6";
    else if (entry.kb_value == K_F7)
        return "F7";
    else if (entry.kb_value == K_F8)
        return "F8";
    else if (entry.kb_value == K_F9)
        return "F9";
    else if (entry.kb_value == K_F10)
        return "F10";
    else if (entry.kb_value == K_F11)
        return "F11";
    else if (entry.kb_value == K_F12)
        return "F12";
    else
        return std::string(1, static_cast<char>(entry.kb_value & 0xFF));

    return "";
}
