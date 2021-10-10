#pragma once

#include <filesystem>
#include <map>
#include <string>

class CConfig
{
  public:
    static std::vector<std::vector<int>> PrintFacelets;
    static std::vector<char> Facelet2Color;
    static int size;

  public:
    static bool Load(std::filesystem::path p);

    //static std::string GetRegex(int level);
};
