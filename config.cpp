#include "config.h"
#include "MagicCube.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "yaml-cpp/yaml.h"

std::map<int, char> CConfig::Dir2ColorMap;
std::map<int, std::map<std::string, SAction>> CConfig::InputsMap;

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

    InputsMap.clear();

    assert(config["inputs"]);
    YAML::Node inputs = config["inputs"];

    for (auto input : inputs)
    {
        int level = 0;

        assert(input["level"]);
        level = input["level"].as<int>();
        assert(level > 0);

        assert(input["actions"]);
        YAML::Node actions = input["actions"];

        for (auto action : actions)
        {
            SAction a;

            assert(action["cmd"]);
            a.cmd = action["cmd"].as<std::string>();
            assert(!a.cmd.empty());

            assert(action["axis"]);
            a.axis = action["axis"].as<int>();
            assert(a.axis >= (int)EAxis::x && a.axis <= (int)EAxis::z);

            assert(action["start"]);
            a.start = action["start"].as<int>();
            assert(std::abs(a.start) < level);
            a.start = (level + a.start) % level;

            assert(action["end"]);
            a.end = action["end"].as<int>();
            assert(std::abs(a.end) < level);
            a.end = (level + a.end) % level;

            assert(action["angle"]);
            a.angle = action["angle"].as<int>();
            assert(a.angle >= (int)EAngle::_0 && a.angle <= (int)EAngle::_270);

            InputsMap[level][a.cmd] = a;
        }
    }
}

std::string CConfig::GetRegex(int level)
{
    std::vector<std::string> v;

    for (auto &s : InputsMap[level])
    {
        v.emplace_back(s.first);
    }

    sort(v.begin(), v.end(), [](std::string &a, std::string &b) { return a.size() > b.size(); });

    std::string regex;

    for (int i = 0; i < v.size(); i++)
    {
        if (i != 0)
        {
            regex += '|';
        }

        regex += v[i];
    }

    return regex;
}
