#include "RubiksCube.h"
#include "config.h"
#include "message.pb.h"

#include <algorithm>
#include <cassert>
#include <list>
#include <queue>
#include <random>

CRubiksCube::CRubiksCube(int size)
{
    assert(size > 0);
    size_ = size;

    facelets_.assign(size, 0);

    for (int i = 0; i < size; i++)
    {
        facelets_[i] = i;
    }
}

bool CRubiksCube::SetFacelets(std::vector<int> &facelets)
{
    if (facelets.size() != size_)
    {
        return false;
    }

    facelets_ = facelets;

    return true;
}

bool CRubiksCube::Rotate(std::string &key)
{
    auto it = CConfig::Key2Transfer.find(key);
    if (it == CConfig::Key2Transfer.end())
    {
        return false;
    }

    for (auto &v : it->second.replace)
    {
        CCubeLogic::DoReplace(facelets_, v);
    }

    return true;
}

void CRubiksCube::show()
{
    for (auto &PrintFacelet : CConfig::PrintFacelets)
    {
        for (auto &i : PrintFacelet)
        {
            if (i < 0 || i >= size_)
            {
                std::cout << ' ';
            }
            else
            {
                std::cout << CConfig::Facelet2Color[facelets_[i]];
            }
        }
        std::cout << std::endl;
    }
}

std::string CRubiksCube::Solve()
{
}

std::string CRubiksCube::RandomRotate(int n)
{
    std::string ans;
    std::vector<std::string> vec;

    for (auto &[k, v] : CConfig::Key2Transfer)
    {
        vec.push_back(k);
    }

    int m = vec.size();

    std::random_device rd;
    std::mt19937 mt(rd());

    for (int i = 0; i < n; i++)
    {
        int r = mt() % m;

        auto s = vec[r];

        ans += s;

        Rotate(s);
    }

    return ans;
}

void CCubeLogic::PrintFacelets(std::vector<int> &facelets)
{
    std::cout << std::endl << "[";

    for (int i = 0; i < facelets.size(); i++)
    {
        if (i > 0)
        {
            std::cout << ",";
        }
        std::cout << facelets[i];
    }

    std::cout << "]" << std::endl << std::endl;
}

void CCubeLogic::DoReplace(std::vector<int> &arr, std::vector<int> &replace)
{
    int n = replace.size();
    if (n <= 1)
    {
        return;
    }

    int temp = arr[replace[n - 1]];

    for (int i = n - 1; i > 0; i--)
    {
        arr[replace[i]] = arr[replace[i - 1]];
    }

    arr[replace[0]] = temp;
}

std::string CCubeLogic::Serialize(std::vector<int> &arr)
{
    Protobuf::Array msg;

    for (auto &i : arr)
    {
        msg.add_array(i);
    }

    return msg.SerializeAsString();
}

std::vector<int> CCubeLogic::Deserialize(std::string &str)
{
    Protobuf::Array msg;

    std::vector<int> arr;

    msg.ParseFromString(str);

    for (auto i : msg.array())
    {
        arr.push_back(i);
    }

    return arr;
}

std::string CCubeLogic::FindShortestPath(const std::string &src, const std::string &dst, int level)
{
    /*std::map<std::string, std::string> path;

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
    }*/

    return "impossible";
}
