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
    std::map<std::pair<int, int>, std::vector<char *>> Rtable_;

  private:
    void InitColors();

    void InitRtable();

    void RotateArray(std::vector<char *> &arr, EAngle angle);

    void Rotate2DArray(std::vector<std::vector<char>> &arrs, EAngle angle);

  public:
    CMagicCube(int level);

    bool SetColors(std::vector<std::vector<std::vector<char>>> &colors);

    bool Rotate(EAxis axis, int start, int end, EAngle angle);

    void print();
};
