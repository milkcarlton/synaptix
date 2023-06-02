#include <pwd.h>
#include <stdexcept>
#include <sys/capability.h>
#include <unistd.h>

#include "disk.h"

DiskUtils::DiskUtils(std::string rootDir) {
    if (rootDir != "")
        this->rootDirectory = rootDir;
    else
        this->rootDirectory = this->rootDir();
}

DiskUtils::DiskUtils() {
    this->rootDirectory = rootDir();
}

DiskUtils::~DiskUtils() { }

std::string DiskUtils::rootDir() {
    if (rootDirectory == "") {
        std::string result;
        try {
	        result = static_cast<std::string>(getenv("XDG_CONFIG_HOME"));
        } catch (std::logic_error e) {
            struct passwd* pw = getpwuid(geteuid());
            if (pw != nullptr) result = static_cast<std::string>(pw->pw_dir) + "/.config";
            else result = "";
        }
        result += "/synaptix";
        rootDirectory = result; 
    }
    return rootDirectory;
}
