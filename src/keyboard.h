#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

#include <string>
#include <unordered_map>

#include "disk.h"

class KeyboardMap {
    public:
        KeyboardMap(DiskUtils* disk);
        KeyboardMap();
        ~KeyboardMap();

        unsigned int getKeyValue(std::string keyStr);
        std::string getKeyStr(unsigned int keyVal);

        void readKeyMap(std::string path);
        void genKeyMapFromSrcDefs(std::string path);

        std::string getXKey(unsigned short keyCode); 
    private:
        DiskUtils* disk;
        int file_descriptor;

        std::unordered_map<std::string, unsigned int> keyMap;
};

#endif
