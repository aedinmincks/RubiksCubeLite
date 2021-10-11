#pragma once

#include <filesystem>
#include <map>
#include <unordered_map>
#include <string>

struct STransfer
{
    std::string key;
    std::vector<std::vector<int>> replace;
    std::string inverse;
};

class CConfig
{
  public:
    static int size;
    static std::vector<std::vector<int>> PrintFacelets;
    static std::vector<char> Facelet2Color;
    static std::unordered_map<std::string, STransfer> Key2Transfer;

  public:
    static bool Load(std::filesystem::path p);

    static std::string GetRegex();
};
