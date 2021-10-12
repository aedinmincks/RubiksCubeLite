#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>

struct STransfer
{
    std::string key;
    std::vector<std::vector<int>> replace;
    std::string inverse;
};

struct SDownGroup
{
    int id;
    std::vector<std::string> group;
    std::vector<std::vector<int>> target;
};

class CConfig
{
  public:
    static int size;
    static std::vector<std::vector<int>> PrintFacelets;
    static std::vector<char> Facelet2Color;
    static std::unordered_map<std::string, STransfer> Key2Transfer;
    static std::vector<SDownGroup> DownGroups;

  public:
    static bool Load(std::filesystem::path p);

    static std::string GetRegex();
};
