#include "MagicCube.h"
#include "config.h"

#include <cassert>
#include <list>
#include <queue>
#include <random>
#include <algorithm>

void CMagicCube::InitColors()
{
    colors_.assign(6 * level_ * level_, ' ');
    for (int i = (int)EDirection::up; i <= (int)EDirection::back; i++)
    {
        for (int j = 0; j < level_; j++)
        {
            for (int k = 0; k < level_; k++)
            {
                colors_[i * level_ * level_ + j * level_ + k] = CConfig::Dir2ColorMap[i];
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

bool CMagicCube::SetColors(std::string &colors)
{
    if (colors.size() != 6 * level_ * level_)
    {
        return false;
    }

    colors_ = colors;

    return true;
}

bool CMagicCube::Rotate(EAxis axis, int start, int end, EAngle angle)
{
    if (start < 0 || start >= level_ || end < 0 || end >= level_)
    {
        return false;
    }

    colors_ = CCubeLogic::RotateColors(colors_, level_, axis, start, end, angle);

    return true;
}

void CMagicCube::show()
{
    int n = level_;

    std::cout << std::endl;

    for (int i = 0; i < n; i++)
    {
        std::cout << std::string(n, ' ');
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::back * n * n + i * n + j];
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::left * n * n + i * n + j];
        }
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::up * n * n + i * n + j];
        }
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::right * n * n + i * n + j];
        }
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::down * n * n + i * n + j];
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < n; i++)
    {
        std::cout << std::string(n, ' ');
        for (int j = 0; j < n; j++)
        {
            std::cout << colors_[(uint8_t)EDirection::front * n * n + i * n + j];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

std::string CMagicCube::RandomRotate(int n)
{
    int m = CConfig::InputsMap[level_].size();

    std::random_device rd;
	std::mt19937 mt(rd());

    std::string ans;

    for (int i = 0; i < n; i++)
    {    
        int r = mt() % m;
       
        auto it = CConfig::InputsMap[level_].begin();
        std::advance(it, r);
        
        ans += it->first;

        Rotate((EAxis)it->second.axis, it->second.start, it->second.end, (EAngle)it->second.angle);
    }

    return ans;
}

bool CCubeLogic::checkColors(const std::string &str, int level)
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

void CCubeLogic::RotateArray(std::string &colors, int level, std::vector<int> &arr, EAngle angle)
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

        temp.emplace_back(colors[n]);
    }

    for (int i = arr.size() - 1; i >= k; i--)
    {
        int n = arr[i];
        int n1 = arr[i - k];

        colors[n] = colors[n1];
    }

    for (int i = 0; i < k; i++)
    {
        int n = arr[i];

        colors[n] = temp[i];
    }
}

void CCubeLogic::Rotate2DArray(char *arrs, int level, EAngle angle)
{
    assert(level > 0);
    assert(arrs);

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
                char temp = arrs[i * level + j];
                arrs[i * level + j] = arrs[(level - 1 - j) * level + i];
                arrs[(level - 1 - j) * level + i] = arrs[(level - 1 - i) * level + (level - 1 - j)];
                arrs[(level - 1 - i) * level + (level - 1 - j)] = arrs[j * level + (level - 1 - i)];
                arrs[j * level + (level - 1 - i)] = temp;
            }
        }
    }
    else if (angle == EAngle::_180)
    {
        for (int i = 0; i < level / 2; i++)
        {
            for (int j = 0; j < level - level / 2; j++)
            {
                std::swap(arrs[i * level + j], arrs[(level - 1 - i) * level + (level - 1 - j)]);
                std::swap(arrs[j * level + (level - 1 - i)], arrs[(level - 1 - j) * level + i]);
            }
        }
    }
    else if (angle == EAngle::_270)
    {
        for (int i = 0; i < level / 2; i++)
        {
            for (int j = 0; j < level - level / 2; j++)
            {
                char temp = arrs[i * level + j];
                arrs[i * level + j] = arrs[j * level + (level - 1 - i)];
                arrs[j * level + (level - 1 - i)] = arrs[(level - 1 - i) * level + (level - 1 - j)];
                arrs[(level - 1 - i) * level + (level - 1 - j)] = arrs[(level - 1 - j) * level + i];
                arrs[(level - 1 - j) * level + i] = temp;
            }
        }
    }
}

std::string CCubeLogic::RotateColors(const std::string &str, int level, EAxis axis, int start, int end, EAngle angle)
{
    assert(level > 0);
    assert(start >= 0 && start < level);
    assert(end >= 0 && end < level);
    assert(str.size() == 6 * level * level);

    std::string colors(str);

    auto m = CConfig::RotateMap[level];

    for (int i = start; i <= end; i++)
    {
        auto k = std::make_pair((int)axis, i);
        if (m.find(k) != m.end())
        {
            auto &v = m[k];
            CCubeLogic::RotateArray(colors, level, v, angle);
        }
    }

    if (start == 0)
    {
        switch (axis)
        {
        case EAxis::x:
            CCubeLogic::Rotate2DArray(&colors[(int)EDirection::back * level * level], level,
                                      (EAngle)((4 - (int)angle) % 4));
            break;
        case EAxis::y:
            CCubeLogic::Rotate2DArray(&colors[(int)EDirection::left * level * level], level,
                                      (EAngle)((4 - (int)angle) % 4));
            break;
        case EAxis::z:
            CCubeLogic::Rotate2DArray(&colors[(int)EDirection::down * level * level], level,
                                      (EAngle)((4 - (int)angle) % 4));
            break;
        default:
            break;
        }
    }

    if (end == level - 1)
    {
        switch (axis)
        {
        case EAxis::x:
            CCubeLogic::Rotate2DArray(&colors[(int)EDirection::front * level * level], level, angle);
            break;
        case EAxis::y:
            CCubeLogic::Rotate2DArray(&colors[(int)EDirection::right * level * level], level, angle);
            break;
        case EAxis::z:
            CCubeLogic::Rotate2DArray(&colors[(int)EDirection::up * level * level], level, angle);
            break;
        default:
            break;
        }
    }

    return colors;
}

std::string CCubeLogic::FindShortestPath(const std::string &src, const std::string &dst, int level)
{
    std::map<std::string, std::string> path;

    std::queue<std::string> q;

    path[src] = {};
    q.push(src);

    int t = 0;

    while (!q.empty())
    {
        int n = q.size();
        t++;

        while (n--)
        {
            auto s = q.front();
            q.pop();

            for (auto &[k, v] : CConfig::InputsMap[level])
            {
                assert(v.axis >= (int)EAxis::x && v.axis <= (int)EAxis::z);
                assert(v.angle >= (int)EAngle::_0 && v.angle <= (int)EAngle::_270);

                std::string s1 = CCubeLogic::RotateColors(s, level, (EAxis)v.axis, v.start, v.end, (EAngle)v.angle);

                if (s1 == dst)
                {
                    return path[s] + k;
                }

                if (path.find(s1) == path.end())
                {
                    path[s1] = path[s] + k;
                    q.push(s1);
                }
            }
        }
    }

    return "impossible";
}
