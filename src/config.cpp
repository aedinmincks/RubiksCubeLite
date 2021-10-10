#include "config.h"
#include "RubiksCube.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "yaml-cpp/yaml.h"

std::vector<char> CConfig::Facelet2Color;
std::vector<std::vector<int>> CConfig::PrintFacelets;
int CConfig::size;

bool CConfig::Load(std::filesystem::path p)
{
    std::string filename = std::filesystem::absolute(p).string();

    printf("load config file <%s> \n", filename.c_str());

    YAML::Node config = YAML::LoadFile(filename);

    if (!config["size"])
    {
        return false;
    }
    size = config["size"].as<int>();

    if (!config["colors"])
    {
        return false;
    }
    YAML::Node colors = config["colors"];

    Facelet2Color.assign(size, ' ');

    for (auto color : colors)
    {
        if (!color["val"])
        {
            return false;
        }
        char val = color["val"].as<char>();

        if (!color["id"])
        {
            return false;
        }
        YAML::Node id = color["id"];

        for (auto i : id)
        {
            int index = i.as<int>();

            if (index < 0 || index >= size)
            {
                return false;
            }
            
            Facelet2Color[index] = val;
        }
    }

    if (!config["print"])
    {
        return false;
    }
    YAML::Node prints = config["print"];

    PrintFacelets.clear();

    for (auto print : prints)
    {
        if (!print["id"])
        {
            return false;
        }
        YAML::Node id = print["id"];

        std::vector<int> v;

        for (auto i : id)
        {
            int index = i.as<int>();

            v.push_back(index);
        }

        PrintFacelets.emplace_back(v);
    }

    return true;
}

//std::string CConfig::GetRegex(int level)
//{
//    std::vector<std::string> v;
//
//    for (auto &s : InputsMap[level])
//    {
//        v.emplace_back(s.first);
//    }
//
//    sort(v.begin(), v.end(), [](std::string &a, std::string &b) { return a.size() > b.size(); });
//
//    std::string regex;
//
//    for (int i = 0; i < v.size(); i++)
//    {
//        if (i != 0)
//        {
//            regex += '|';
//        }
//
//        regex += v[i];
//    }
//
//    return regex;
//}
