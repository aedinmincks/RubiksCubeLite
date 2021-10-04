#include "MagicCube.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <cassert>

#include "yaml-cpp/yaml.h"


std::map<int, char> CConfig::Dir2ColorMap;

void CConfig::Load(std::filesystem::path p)
{
	p /= std::filesystem::path("config.yaml");

	std::string filename = std::filesystem::absolute(p).string();

	printf("load config file <%s> \n", filename.c_str());

	YAML::Node config = YAML::LoadFile(filename);

	assert(config["colors"]);
	YAML::Node colors = config["colors"];

	Dir2ColorMap.clear();

	assert(colors["up"]);
	Dir2ColorMap[(int)EDirection::up] = colors["up"].as<char>();

	assert(colors["down"]);
	Dir2ColorMap[(int)EDirection::down] = colors["down"].as<char>();

	assert(colors["left"]);
	Dir2ColorMap[(int)EDirection::left] = colors["left"].as<char>();

	assert(colors["right"]);
	Dir2ColorMap[(int)EDirection::right] = colors["right"].as<char>();

	assert(colors["front"]);
	Dir2ColorMap[(int)EDirection::front] = colors["front"].as<char>();

	assert(colors["back"]);
	Dir2ColorMap[(int)EDirection::back] = colors["back"].as<char>();

}
