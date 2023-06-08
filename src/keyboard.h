#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

#include <string>
#include <unordered_map>

#include "disk.h"

class KeyboardMap {
    public:
        explicit KeyboardMap(DiskUtils* disk);
        ~KeyboardMap();
        
        unsigned int getKeyValue(std::string keyStr);
        std::string getKeyAlias(unsigned int keyVal);

        bool sourceKeyMap(const std::string& path);
        void genKeyMapFromSrcDefs(bool show = false, std::string path = "/usr/include/linux/input-event-codes.h");

    private:
        DiskUtils* disk;
        std::unordered_map<std::string, unsigned int> keyMap;

        void insertKey(std::string keyAlias, unsigned int keyVal);
};

#endif
