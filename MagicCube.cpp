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

void CMagicCube::InitRtable()
{
    Rtable_.clear();

    for (int i = 0; i < level_; i++)
    {
        auto k = std::make_pair((int)EAxis::x, i);
        auto &v = Rtable_[k];

        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::left][i][j]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::up][i][j]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::right][i][j]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::down][i][j]);
        }
    }

    for (int i = 0; i < level_; i++)
    {
        auto k = std::make_pair((int)EAxis::y, i);
        auto &v = Rtable_[k];

        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::front][level_ - 1 - j][i]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::up][level_ - 1 - j][i]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::back][level_ - 1 - j][i]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::down][j][level_ - 1 - i]);
        }
    }

    for (int i = 0; i < level_; i++)
    {
        auto k = std::make_pair((int)EAxis::z, i);
        auto &v = Rtable_[k];

        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::left][level_ - 1 - j][i]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::back][i][j]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::right][j][level_ - 1 - i]);
        }
        for (int j = 0; j < level_; j++)
        {
            v.emplace_back(&colors_[(int)EDirection::front][level_ - 1 - i][level_ - 1 - j]);
        }
    }
}

void CMagicCube::RotateArray(std::vector<char *> &arr, EAngle angle)
{
    assert(arr.size() == level_ * 4);

    if (angle == EAngle::_0)
    {
        return;
    }

    int k = (int)angle * level_;

    std::vector<char> temp;

    for (int i = arr.size() - k; i < arr.size(); i++)
    {
        temp.emplace_back(*arr[i]);
    }

    for (int i = arr.size() - 1; i >= k; i--)
    {
        *arr[i] = *arr[i - k];
    }

    for (int i = 0; i < k; i++)
    {
        *arr[i] = temp[i];
    }
}

void CMagicCube::Rotate2DArray(std::vector<std::vector<char>> &arrs, EAngle angle)
{
    assert(arrs.size() == level_);
    assert(arrs[0].size() == level_);

    if (angle == EAngle::_0)
    {
        // do nothing
    }
    else if (angle == EAngle::_90)
    {
        for (int i = 0; i < level_ / 2; i++)
        {
            for (int j = 0; j < level_ - level_ / 2; j++)
            {
                char temp = arrs[i][j];
                arrs[i][j] = arrs[level_ - 1 - j][i];
                arrs[level_ - 1 - j][i] = arrs[level_ - 1 - i][level_ - 1 - j];
                arrs[level_ - 1 - i][level_ - 1 - j] = arrs[j][level_ - 1 - i];
                arrs[j][level_ - 1 - i] = temp;
            }
        }
    }
    else if (angle == EAngle::_180)
    {
        for (int i = 0; i < level_ / 2; i++)
        {
            for (int j = 0; j < level_ - level_ / 2; j++)
            {
                std::swap(arrs[i][j], arrs[level_ - 1 - i][level_ - 1 - j]);
                std::swap(arrs[j][level_ - 1 - i], arrs[level_ - 1 - j][i]);
            }
        }
    }
    else if (angle == EAngle::_270)
    {
        for (int i = 0; i < level_ / 2; i++)
        {
            for (int j = 0; j < level_ - level_ / 2; j++)
            {
                char temp = arrs[i][j];
                arrs[i][j] = arrs[j][level_ - 1 - i];
                arrs[j][level_ - 1 - i] = arrs[level_ - 1 - i][level_ - 1 - j];
                arrs[level_ - 1 - i][level_ - 1 - j] = arrs[level_ - 1 - j][i];
                arrs[level_ - 1 - j][i] = temp;
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
    InitRtable();
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

    std::vector<std::vector<std::vector<char>>> copy(colors_);

    for (int i = start; i <= end; i++)
    {
        auto k = std::make_pair((int)axis, i);
        if (Rtable_.find(k) != Rtable_.end())
        {
            auto &v = Rtable_[k];
            RotateArray(v, angle);
        }
    }

    if (start == 0)
    {
        switch (axis)
        {
        case EAxis::x:
            Rotate2DArray(colors_[(int)EDirection::back], (EAngle)((4 - (int)angle) % 4));
            break;
        case EAxis::y:
            Rotate2DArray(colors_[(int)EDirection::left], (EAngle)((4 - (int)angle) % 4));
            break;
        case EAxis::z:
            Rotate2DArray(colors_[(int)EDirection::down], (EAngle)((4 - (int)angle) % 4));
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
            Rotate2DArray(colors_[(int)EDirection::front], angle);
            break;
        case EAxis::y:
            Rotate2DArray(colors_[(int)EDirection::right], angle);
            break;
        case EAxis::z:
            Rotate2DArray(colors_[(int)EDirection::up], angle);
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
