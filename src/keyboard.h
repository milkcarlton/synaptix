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
        std::string getKeyAlias(unsigned int keyVal);

        bool sourceKeyMap(std::string path);
        void genKeyMapFromSrcDefs(bool show = false, std::string path = "/usr/include/linux/input-event-codes.h");

    private:
        DiskUtils* disk;
        int file_descriptor;
        std::unordered_map<std::string, unsigned int> keyMap;

        void insertKey(std::string keyAlias, unsigned int keyVal);
};

#endif
