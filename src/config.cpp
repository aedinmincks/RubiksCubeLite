#include "config.h"
#include "RubiksCube.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "yaml-cpp/yaml.h"

int CConfig::size;
std::vector<char> CConfig::Facelet2Color;
std::vector<std::vector<int>> CConfig::PrintFacelets;
std::unordered_map<std::string, STransfer> CConfig::Key2Transfer;
std::vector<SDownGroup> CConfig::DownGroups;

bool CConfig::Load(std::filesystem::path p)
{
    std::string filename = std::filesystem::absolute(p).string();

    printf("load config file <%s> \n", filename.c_str());

    YAML::Node config = YAML::LoadFile(filename);

    if (!config["size"])
    {
        return false;
    }
    size = config["size"].as<int>();

    if (!config["colors"])
    {
        return false;
    }
    YAML::Node colors = config["colors"];

    Facelet2Color.assign(size, ' ');

    for (auto color : colors)
    {
        if (!color["val"])
        {
            return false;
        }
        char val = color["val"].as<char>();

        if (!color["id"])
        {
            return false;
        }
        YAML::Node id = color["id"];

        for (auto i : id)
        {
            int index = i.as<int>();

            if (index < 0 || index >= size)
            {
                printf("the index<%d> does not exist in colors val<%c>\r\n", index, val);
                return false;
            }
            
            Facelet2Color[index] = val;
        }
    }

    if (!config["print"])
    {
        return false;
    }
    YAML::Node prints = config["print"];

    PrintFacelets.clear();

    for (auto print : prints)
    {
        if (!print["id"])
        {
            return false;
        }
        YAML::Node id = print["id"];

        std::vector<int> v;

        for (auto i : id)
        {
            int index = i.as<int>();

            v.push_back(index);
        }

        PrintFacelets.emplace_back(v);
    }
    
    if (!config["transfer"])
    {
        return false;
    }
    YAML::Node transfers = config["transfer"];

    for (auto transfer : transfers)
    {
        STransfer st;

        if (!transfer["key"])
        {
            return false;
        }
        st.key = transfer["key"].as<std::string>();
        
        if (!transfer["replace"])
        {
            return false;
        }
        YAML::Node replace = transfer["replace"];

        for (auto i : replace)
        {
            std::vector<int> v;

            for (auto j : i)
            {
                int index = j.as<int>();

                if (index < 0 || index >= size)
                {
                    printf("the index<%d> does not exist in transfer key<%s>\r\n", index, st.key.c_str());
                    return false;
                }

                v.emplace_back(index);
            }
            
            st.replace.emplace_back(v);
        }

        if (!transfer["inverse"])
        {
            return false;
        }
        st.inverse = transfer["inverse"].as<std::string>();

        Key2Transfer.emplace(st.key, st);
    }

    if (!config["downgroup"])
    {
        return false;
    }
    YAML::Node downgroups = config["downgroup"];

    for (auto downgroup : downgroups)
    {
        SDownGroup dg;

        if (!downgroup["id"])
		{
			return false;
		}
        dg.id = downgroup["id"].as<int>();
        
        if (!downgroup["group"])
        {
            return false;
        }
        
        for (auto str : downgroup["group"])
        {
            std::string key = str.as<std::string>();
            
            if (Key2Transfer.find(key) == Key2Transfer.end())
            {
                printf("the key<%s> does not exist in downgroup id<%d>\r\n", key.c_str(), dg.id);
                return false;
            }

            dg.group.emplace_back(key);
        }
        
        if (!downgroup["target"])
        {
            return false;
        }

        for (auto i : downgroup["target"])
        {
            std::vector<int> v;

            for (auto j : i)
            {
                int index = j.as<int>();

                if (index < 0 || index >= size)
                {
                    printf("the index<%d> does not exist in downgroup id<%d>\r\n", index, dg.id);
                    return false;
                }

                v.emplace_back(index);
            }

            dg.target.emplace_back(v);
        }

        DownGroups.emplace_back(dg);
    }

    return true;
}

std::string CConfig::GetRegex()
{
    std::vector<std::string> v;

    for (auto &s : Key2Transfer)
    {
        v.emplace_back(s.first);
    }

    sort(v.begin(), v.end(), [](std::string &a, std::string &b) { return a.size() > b.size(); });

    std::string regex;

    for (int i = 0; i < v.size(); i++)
    {
        if (i != 0)
        {
            regex += '|';
        }

        regex += v[i];
    }

    return regex;
}
