#pragma once

#include <filesystem>
#include <map>
#include <string>

struct SAction
{
    std::string cmd;
    int axis;
    int start;
    int end;
    int angle;
};

class CConfig
{
  public:
    static std::map<int, char> Dir2ColorMap;
    static std::map<int, std::map<std::string, SAction>> InputsMap;

  public:
    static void Load(std::filesystem::path p);

    static std::string GetRegex(int level);
};
