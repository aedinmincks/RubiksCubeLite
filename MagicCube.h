#pragma once

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

/*
 *	  z
 *	  |
 *	  |___y
 *	 /
 *	/x
 *
 *	沿x轴顺时针旋转为正方向（z->y）,以此类推。
 */

enum class EAxis : uint8_t
{
    x = 0,
    y,
    z,
};

enum class EDirection : uint8_t
{
    up = 0,
    down,
    left,
    right,
    front,
    back,
};

enum class EAngle : uint8_t
{
    _0 = 0,
    _90,
    _180,
    _270,
};

class CMagicCube
{
  public:
    int level_;
    std::string colors_;

  private:
    void InitColors();

  public:
    CMagicCube(int level);

    bool SetColors(std::string &colors);

    std::string &GetColors(void)
    {
        return colors_;
    };

    bool Rotate(EAxis axis, int start, int end, EAngle angle);

    void show();

    std::string RandomRotate(int n);
};

class CCubeLogic
{
  public:
    static bool checkColors(const std::string &str, int level);

    static void RotateArray(std::string &colors, int level, std::vector<int> &arr, EAngle angle);

    static void Rotate2DArray(char *colors, int level, EAngle angle);

    static std::string RotateColors(const std::string &str, int level, EAxis axis, int start, int end, EAngle angle);

    static std::string FindShortestPath(const std::string &src, const std::string &dst, int level);
};
