#pragma once

#include <filesystem>

class CConfig {
public:
	static void Load(std::filesystem::path p);
};
