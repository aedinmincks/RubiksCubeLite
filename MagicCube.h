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
    std::vector<std::vector<std::vector<char>>> colors_;

  private:
    void InitColors();

  public:
    CMagicCube(int level);

    bool SetColors(std::vector<std::vector<std::vector<char>>> &colors);

    std::vector<std::vector<std::vector<char>>> &GetColors(void)
    {
        return colors_;
    };

    bool Rotate(EAxis axis, int start, int end, EAngle angle);

    void print();
};

class CCodec
{
  public:
    static std::string Serialization(const std::vector<std::vector<std::vector<char>>> &colors);

    static std::vector<std::vector<std::vector<char>>> Deserialization(const std::string &str);

    static std::vector<std::vector<std::vector<char>>> Deserialization(const std::string &str, int level);

    static bool checkColors(const std::string &str, int level);
};

class CCubeLogic
{
  public:
    static void RotateArray(std::vector<std::vector<std::vector<char>>> &colors, int level, std::vector<int> &arr,
                            EAngle angle);

    static void Rotate2DArray(std::vector<std::vector<char>> &arrs, int level, EAngle angle);

    static std::string RotateColors(const std::string &str, int level, EAxis axis, int start, int end, EAngle angle);
};
