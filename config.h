#pragma once

#include <filesystem>
#include <map>

class CConfig {
public:
	static std::map<int, char> Dir2ColorMap;

public:
	static void Load(std::filesystem::path p);


};
