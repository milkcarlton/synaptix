#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

#include <string>
#include <unordered_map>

class KeyboardMap {
    public:
        KeyboardMap();
        ~KeyboardMap();

        std::string getKey(unsigned short keyCode); 
        bool hasKeyVal(unsigned short keyCode);
        void assignKey(unsigned short keyCode, char character);
    private:
        std::unordered_map<unsigned short, char> keyMap;
        int file_descriptor;
};

#endif
