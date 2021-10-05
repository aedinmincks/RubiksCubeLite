#include "MagicCube.h"
#include "config.h"

#include <cassert>

void CMagicCube::InitColors()
{
    colors_.assign(6, std::vector<std::vector<char>>(level_, std::vector<char>(level_, ' ')));
    for (int i = (int)EDirection::up; i <= (int)EDirection::back; i++)
    {
        for (int j = 0; j < level_; j++)
        {
            for (int k = 0; k < level_; k++)
            {
                colors_[i][j][k] = CConfig::Dir2ColorMap[i];
            }
        }
    }
}

CMagicCube::CMagicCube(int level)
{
    if (level < 1)
    {
        level = 3;
    }

    level_ = level;

    InitColors();
}

bool CMagicCube::SetColors(std::vector<std::vector<std::vector<char>>> &colors)
{
    if (colors.size() != 6)
    {
        return false;
    }

    if (colors[0].size() != level_)
    {
        return false;
    }

    if (colors[0][0].size() != level_)
    {
        return false;
    }

    for (int i = (int)EDirection::up; i <= (int)EDirection::back; i++)
    {
        for (int j = 0; j < level_; j++)
        {
            for (int k = 0; k < level_; k++)
            {
                colors_[i][j][k] = colors[i][j][k];
            }
        }
    }

    return true;
}

bool CMagicCube::Rotate(EAxis axis, int start, int end, EAngle angle)
{
    if (start < 0 || start >= level_ || end < 0 || end >= level_)
    {
        return false;
    }

    auto m = CConfig::RotateMap[level_];

    for (int i = start; i <= end; i++)
    {
        auto k = std::make_pair((int)axis, i);
        if (m.find(k) != m.end())
        {
            auto &v = m[k];
            CCubeLogic::RotateArray(colors_, level_, v, angle);
        }
    }

    if (start == 0)
    {
        switch (axis)
        {
        case EAxis::x:
            CCubeLogic::Rotate2DArray(colors_[(int)EDirection::back], level_, (EAngle)((4 - (int)angle) % 4));
            break;
        case EAxis::y:
            CCubeLogic::Rotate2DArray(colors_[(int)EDirection::left], level_, (EAngle)((4 - (int)angle) % 4));
            break;
        case EAxis::z:
            CCubeLogic::Rotate2DArray(colors_[(int)EDirection::down], level_, (EAngle)((4 - (int)angle) % 4));
            break;
        default:
            break;
        }
    }

    if (end == level_ - 1)
    {
        switch (axis)
        {
        case EAxis::x:
            CCubeLogic::Rotate2DArray(colors_[(int)EDirection::front], level_, angle);
            break;
        case EAxis::y:
            CCubeLogic::Rotate2DArray(colors_[(int)EDirection::right], level_, angle);
            break;
        case EAxis::z:
            CCubeLogic::Rotate2DArray(colors_[(int)EDirection::up], level_, angle);
            break;
        default:
            break;
        }
    }

    return true;
}

void CMagicCube::print()
{
    int n = level_;

    std::cout << std::endl;

    for (int i = 0; i < n; i++)
    {
        std::cout << std::string(n, ' ');
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::back][i][j];
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::left][i][j];
        }
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::up][i][j];
        }
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::right][i][j];
        }
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::down][i][j];
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < n; i++)
    {
        std::cout << std::string(n, ' ');
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::front][i][j];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

std::string CCodec::Serialization(const std::vector<std::vector<std::vector<char>>> &colors)
{
    std::string str;

    for (auto &i : colors)
    {
        for (auto &j : i)
        {
            for (auto &k : j)
            {
                str += k;
            }
        }
    }

    return str;
}

std::vector<std::vector<std::vector<char>>> CCodec::Deserialization(const std::string &str)
{
    int level = std::sqrt(str.size() / 6);

    return Deserialization(str, level);
}

std::vector<std::vector<std::vector<char>>> CCodec::Deserialization(const std::string &str, int level)
{
    assert(level > 0);
    assert(str.size() >= level * level * 6);

    std::vector<std::vector<std::vector<char>>> colors;

    colors.assign(6, std::vector<std::vector<char>>(level, std::vector<char>(level, ' ')));

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < level; j++)
        {
            for (int k = 0; k < level; k++)
            {
                colors[i][j][k] = str[i * level * level + j * level + k];
            }
        }
    }

    return colors;
}

bool CCodec::checkColors(const std::string &str, int level)
{
    if (str.size() != level * level * 6)
    {
        return false;
    }

    std::map<char, int> m;
    for (auto &c : str)
    {
        m[c]++;
    }

    for (auto i = 0; i < 6; i++)
    {
        if (m[CConfig::Dir2ColorMap[i]] != level * level)
        {
            return false;
        }
    }

    return true;
}

void CCubeLogic::RotateArray(std::vector<std::vector<std::vector<char>>> &colors, int level, std::vector<int> &arr,
                             EAngle angle)
{
    assert(level > 0);
    assert(arr.size() == level * 4);

    if (angle == EAngle::_0)
    {
        return;
    }

    int k = (int)angle * level;

    std::vector<char> temp;

    for (int i = arr.size() - k; i < arr.size(); i++)
    {
        int n = arr[i];
        int d = n / (level * level);
        int x = n % (level * level) / level;
        int y = n % (level * level) % level;

        temp.emplace_back(colors[d][x][y]);
    }

    for (int i = arr.size() - 1; i >= k; i--)
    {
        int n = arr[i];
        int d = n / (level * level);
        int x = n % (level * level) / level;
        int y = n % (level * level) % level;

        int n1 = arr[i - k];
        int d1 = n1 / (level * level);
        int x1 = n1 % (level * level) / level;
        int y1 = n1 % (level * level) % level;

        colors[d][x][y] = colors[d1][x1][y1];
    }

    for (int i = 0; i < k; i++)
    {
        int n = arr[i];
        int d = n / (level * level);
        int x = n % (level * level) / level;
        int y = n % (level * level) % level;

        colors[d][x][y] = temp[i];
    }
}

void CCubeLogic::Rotate2DArray(std::vector<std::vector<char>> &arrs, int level, EAngle angle)
{
    assert(level > 0);
    assert(arrs.size() == level);
    assert(arrs[0].size() == level);

    if (angle == EAngle::_0)
    {
        // do nothing
    }
    else if (angle == EAngle::_90)
    {
        for (int i = 0; i < level / 2; i++)
        {
            for (int j = 0; j < level - level / 2; j++)
            {
                char temp = arrs[i][j];
                arrs[i][j] = arrs[level - 1 - j][i];
                arrs[level - 1 - j][i] = arrs[level - 1 - i][level - 1 - j];
                arrs[level - 1 - i][level - 1 - j] = arrs[j][level - 1 - i];
                arrs[j][level - 1 - i] = temp;
            }
        }
    }
    else if (angle == EAngle::_180)
    {
        for (int i = 0; i < level / 2; i++)
        {
            for (int j = 0; j < level - level / 2; j++)
            {
                std::swap(arrs[i][j], arrs[level - 1 - i][level - 1 - j]);
                std::swap(arrs[j][level - 1 - i], arrs[level - 1 - j][i]);
            }
        }
    }
    else if (angle == EAngle::_270)
    {
        for (int i = 0; i < level / 2; i++)
        {
            for (int j = 0; j < level - level / 2; j++)
            {
                char temp = arrs[i][j];
                arrs[i][j] = arrs[j][level - 1 - i];
                arrs[j][level - 1 - i] = arrs[level - 1 - i][level - 1 - j];
                arrs[level - 1 - i][level - 1 - j] = arrs[level - 1 - j][i];
                arrs[level - 1 - j][i] = temp;
            }
        }
    }
}

std::string CCubeLogic::RotateColors(const std::string &str, int level, EAxis axis, int start, int end, EAngle angle)
{
    assert(level > 0);
    assert(start >= 0 && start < level);
    assert(end >= 0 && end < level);

    std::string ans;

    return ans;
}
