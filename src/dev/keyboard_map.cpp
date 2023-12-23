#include "keyboard_map.h"
#include <ctype.h>
#include <algorithm>
#include <cstdio>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include "../io/disk.h"

KeyboardMap::KeyboardMap(DiskUtils* disk) {
	this->disk = disk;
	if (!this->sourceKeyMap(disk->rootDir()))
		this->genKeyMapFromSrcDefs();
}

KeyboardMap::~KeyboardMap() { }

unsigned int KeyboardMap::getKeyValue(std::string keyStr) {
	if (keyMap.count(keyStr))
	   return keyMap.at(keyStr); 
	
	return 0;
}

std::string KeyboardMap::getKeyAlias(unsigned int keyVal) {
	std::unordered_map<std::string, unsigned int>::iterator it; 
	it = std::find_if(keyMap.begin(), keyMap.end(), 
		[&keyVal](const std::unordered_map<std::string, unsigned int>::value_type& key) -> bool { 
			return key.second == keyVal; 
	});

	if (it != keyMap.end())
		return it->first; 
	else
		return "Not Found";
}

void KeyboardMap::insertKey(std::string keyAlias, unsigned int keyVal) {
	for (size_t i = 0; i < keyAlias.length(); i++) keyAlias[i] = tolower(keyAlias[i]);

	keyMap.insert(std::pair<std::string, unsigned int>(keyAlias, keyVal));
}

void KeyboardMap::genKeyMapFromSrcDefs(bool show, std::string path) {
	std::ifstream defStream(path);

	if (defStream.is_open()) {
		std::string line;
	
		while (std::getline(defStream, line)) {
			if (!line.starts_with("#define")) continue;
			bool isKey = line.starts_with("#define KEY_");
			bool isBtn = line.starts_with("#define BTN_");
			if (!isKey && !isBtn) continue;
			
			char delim = (isKey) ? '_' : ' ';
			line = line.substr(line.find(delim) + 1);
	
			std::string tok;
			std::istringstream ss(line);
			std::vector<std::string> tokens;
	
			while (ss >> tok) tokens.push_back(tok);
			if (tokens.size() < 2) continue;
			
			std::string keyAlias = tokens[0];
			unsigned int keyVal = 0;

			try {
				if (tokens[1].starts_with("0x"))
					keyVal = std::stoul(tokens[1], 0, 16);
				else
					keyVal = std::stoul(tokens[1]);
			} catch(const std::exception &err) {
				continue;
			}
			this->insertKey(keyAlias, keyVal);
		}
		defStream.close();

		if (show) {
			for (auto key : keyMap) {
				std::cout << key.first << "\t" << key.second << std::endl;
			}
		}
	} else {
		std::cout << "Failed to generate key map!" << std::endl;
	}
}

bool KeyboardMap::sourceKeyMap(const std::string& path) {
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
			for (size_t i = 0; i < key.length(); i++)
				key[i] = tolower(key[i]);
			
			keyMap.insert(std::pair<std::string, unsigned int>(
				key,
				std::stoul(tokens[1])
			));
		}
		keyConfig.close();
	} else {
		return false;
	}
	return true;
}

