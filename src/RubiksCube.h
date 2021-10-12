#pragma once

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

    std::string RandomRotate(int n);
};

class CCubeLogic
{
  public:
    static void PrintFacelets(std::vector<int> &facelets);

    static void DoReplace(std::vector<int> &arr, std::vector<int> &replace);

    static std::string Serialize(std::vector<int> &arr);

    static std::vector<int> Deserialize(std::string &str);

    static std::string FindShortestPath(const std::string &src, const std::string &dst, int level);
};
