#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sys/time.h>

#include <unistd.h>
#include <sys/capability.h>

#include "macro.h"
#include "device.h"
#include "loader.h"
#include "device_manager.h"

using std::string, std::cout, std::endl;

char getSafeChar(std::string str, short pos) {
    if (pos >= 0 && pos < str.size()) return str.at(pos);
    return 0;
}

int parseRequest(int argc, int argn, char** argv) {
    DeviceManager dm;
    Loader loader;

    int nextArg = argn;
    char fchar, schar, tchar;
    std::string str = "";

    if (argn <= argc) str = argv[argn];
    fchar = getSafeChar(str, 0);
    schar = getSafeChar(str, 1);
    tchar = getSafeChar(str, 2);
    
    seteuid(0);
    if (fchar == '-') {
        switch (schar) {
            case 'l':
                loader.load(dm);
                while (true) {}
                break; 
            case 'd':
                if (argn < argc) {
                    std::string devicePath = argv[argn + 1];
                    
                    struct ActivatorBind activator = { 1, 272, HELD };
                    Macro m = Macro("TestMacro", activator);
                    m.addResponse(XK_R, 0, 0);
                    m.addResponse(XK_Q, 0, 0);
                    dm.bindMacro(devicePath, m);
                    
                    while (true) {} 
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
    //std::cout << "EUID: " << geteuid() << " UID: " << getuid() << "\n";
    uid_t euid = geteuid();
    
    int nextArg = 0;
    while (true) {
        if (nextArg == -1) break;
        nextArg = parseRequest(argc, nextArg, argv);
    }
 
	return 0;
}



