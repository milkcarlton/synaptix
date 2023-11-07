#ifndef DISK_UTILS_H
#define DISK_UTILS_H

#include <string>

class DiskUtils {
		public:
				explicit DiskUtils(const std::string& rootDir);
				DiskUtils();
				~DiskUtils();

		std::string rootDir();
		private:
		std::string rootDirectory = "";
};

#endif
