#include "RubiksCube.h"
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
    std::string ans;

    std::vector<int> arr(facelets_);

    for (auto dg : CConfig::DownGroups)
    {
        auto s = CCubeLogic::FindShortestPath(arr, arr, dg);
        
        printf("%s\r\n", s.c_str());

        CCubeLogic::PrintFacelets(arr);

        if (s == "impossible")
        {
            return "impossible";
        }

        ans += s;
    }

    return ans;
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

std::vector<int> CCubeLogic::DoReplacesByKey(const std::vector<int> &arr, std::string key)
{
    std::vector<int> ans(arr);

    auto it = CConfig::Key2Transfer.find(key);
    if (it == CConfig::Key2Transfer.end())
    {
        return ans;
    }

    for (auto &v : it->second.replace)
    {
        CCubeLogic::DoReplace(ans, v);
    }

    return ans;
}

std::string CCubeLogic::Serialize(const std::vector<int> &arr)
{
    Protobuf::Array msg;

    for (auto &i : arr)
    {
        msg.add_array(i);
    }

    return msg.SerializeAsString();
}

std::vector<int> CCubeLogic::Deserialize(const std::string &str)
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

bool CCubeLogic::IsGroupTarget(const std::vector<int> &arr, const SDownGroup &dg)
{
    for (auto &v : dg.target)
    {
        std::set<int> st(v.begin(), v.end());

        for (auto &i : v)
        {
            if (st.find(arr[i]) == st.end())
            {
                return false;
            }
        }
    }

    return true;
}

std::string CCubeLogic::FindShortestPath(const std::vector<int> &src, std::vector<int> &dst, const SDownGroup &dg)
{
    std::unordered_map<std::string, std::string> path;
    std::queue<std::string> q;

    if (IsGroupTarget(src, dg))
    {
        dst = src;
        return "";
    }

    auto str_src = CCubeLogic::Serialize(src);

    path[str_src] = {};
    q.push(str_src);

    int t = 0;

    while (!q.empty())
    {
        int n = q.size();
        t++;

        while (n--)
        {
            auto s = q.front();
            q.pop();

            auto vec = Deserialize(s);

            for (auto &key : dg.group)
            {
                auto arr = CCubeLogic::DoReplacesByKey(vec, key);

                if (IsGroupTarget(arr, dg))
                {
                    dst = arr;
                    return path[s] + key;
                }

                auto s1 = CCubeLogic::Serialize(arr);

                if (path.find(s1) == path.end())
                {
                    path[s1] = path[s] + key;
                    q.push(s1);
                }
            }
        }
    }

    return "impossible";
}
