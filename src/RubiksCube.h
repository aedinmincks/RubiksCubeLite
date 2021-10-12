#pragma once

#include "config.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class CRubiksCube
{
  public:
    std::vector<int> facelets_;
    int size_;

  public:
    CRubiksCube(int size);

    bool SetFacelets(std::vector<int> &facelets);

    std::vector<int> &GetFacelets(void)
    {
        return facelets_;
    };

    bool Rotate(std::string &cmd);

    void show();

    std::string Solve();

    std::string RandomRotate(int n);
};

class CCubeLogic
{
  public:
    static void PrintFacelets(std::vector<int> &facelets);

    static void DoReplace(std::vector<int> &arr, std::vector<int> &replace);

    static std::vector<int> DoReplacesByKey(const std::vector<int> &arr, std::string key);

    static std::string Serialize(const std::vector<int> &arr);

    static std::vector<int> Deserialize(const std::string &str);

    static bool IsGroupTarget(const std::vector<int> &arr, const SDownGroup &dg);

    static std::string FindShortestPath(const std::vector<int> &src, std::vector<int> &dst, const SDownGroup &dg);
};
