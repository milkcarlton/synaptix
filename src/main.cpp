#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sys/time.h> 

#include "macro.h"
#include "device.h"
#include "device_manager.h"

using std::string, std::cout, std::endl;

std::string rootDir() {
	return static_cast<std::string>(getenv("XDG_CONFIG_HOME")) + "/synaptix";
}

char getSafeChar(std::string str, short pos) {
    if (pos >= 0 && pos < str.size()) return str.at(pos);
    return 0;
}

int parseRequest(int argc, int argn, char** argv) {
    DeviceManager dm;
    int nextArg = argn;
    char fchar, schar, tchar;
    std::string str = "";

    if (argn <= argc) str = argv[argn];
    fchar = getSafeChar(str, 0);
    schar = getSafeChar(str, 1);
    tchar = getSafeChar(str, 2);
    
    if (fchar == '-') {
        switch (schar) {
            case 'd':
                if (argn < argc) {
                    std::string devicePath = argv[argn + 1];
                    
                    Macro m = Macro("TestMacro", 1, 272, HELD);
                    m.addResponse(XK_R, 0, 0);
                    m.addResponse(XK_Q, 0, 0);
                    dm.bindMacro(devicePath, m);
                    
                    while (true) {} 
                    nextArg++; 
                }
                break;
            case 'b':
                if (argn < argc) {
	                std::cout << "Loading Macros Test..." << std::endl;
                    // Create as separate thread
                    MacroDevice mdev = MacroDevice(argv[argn + 1]);
                    Macro m = Macro("TestMacro", 1, 272, RELEASE);
                    m.addResponse(XK_E, 0, 0);
                    mdev.registerMacro(m);
                    mdev.toggleMacros(true);
                    mdev.governMacros();
                    nextArg++; 
                }
                break;
            case 'm':
                if (argn < argc) {
		            std::cout << "Monitoring Device Input..." << std::endl;
                    MacroDevice mdev = MacroDevice(argv[argn + 1]);
                    mdev.inspectDevice(-1);
                    nextArg++; 
                }
                break; 
            case 'h':
                std::cout << "/=========[SYNAPTIX HELP]=========\\" << std::endl;
                std::cout << "-m [path]: Monitor device input" << std::endl;
                std::cout << "\tex: -m /dev/input/event10" << std::endl;
                std::cout << "-d [path]: Create test device macro" << std::endl;
                std::cout << "\tex: -t /dev/input/event10" << std::endl;
                break; 
            default:
                std::cout << "Improper command usage. Run with -h for help!" << std::endl;
                nextArg++;
        }
    }
    nextArg++;
    if (nextArg >= argc) nextArg = -1; 
    return nextArg;
}

int main(int argc, char** argv) {
    std::cout << "Initializing Macros.." << std::endl;
    std::string devicePath = "/dev/input/event10";
     
    // TODO: set device & function flag separately
    int nextArg = 0;
    while (true) {
        if (nextArg == -1) break;
        nextArg = parseRequest(argc, nextArg, argv);
    }
 
	return 0;
}



