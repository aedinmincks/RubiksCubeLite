// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "VCube2.h"
#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <regex>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "argc is not 2" << std::endl;
        return 1;
    }

    std::unique_ptr<CRubiksCubeBase> cube;

    std::string CubeString(argv[1]);
    if (CubeString == "2")
    {
        cube = std::make_unique<VCube2>();
    }
    else
    {
        std::cout << "argv[1] is not exist" << std::endl;
        return 2;
    }

    while (1)
    {
        std::string cmd;

        std::cout << "#";
        std::cin >> cmd;

        if (cmd == "exit" || cmd == "quit")
        {
            std::cout << "Good bye!\n";
            break;
        }
        else if (cmd == "help")
        {
            std::cout << "just see code.\n";
        }
        else if (cmd == "show")
        {
            cube->Show();
        }
        else if (cmd == "do")
        {
            std::string s;
            std::cin >> s;

            std::regex r(cube->GetRegex());
            std::smatch sm;
            while (std::regex_search(s, sm, r))
            {
                std::string key = sm.str();

                std::cout << key << std::endl;

                auto state = cube->ApplyMove(cube->GetMove(key), cube->GetState());

                cube->SetState(state);

                s = sm.suffix();
            }
        }
        else if (cmd == "get")
        {
            std::vector<std::string> vs;
            if (cube->State2String(cube->GetState(), vs))
            {
                std::cout << "[";

                for (int i = 0; i < vs.size() - 1; i++)
                {
                    std::cout << vs[i] << ",";
                }

                std::cout << vs.back() << "]" << std::endl;
            }
            else
            {
                std::cout << "fail" << std::endl;
            }
        }
        else if (cmd == "set")
        {
            std::string s;
            std::cin >> s;

            std::vector<std::string> vs;

            std::regex r(R"([A-Z]+)");
            std::smatch sm;
            while (std::regex_search(s, sm, r))
            {
                vs.emplace_back(sm.str());

                s = sm.suffix();
            }

            vi state;

            if (cube->String2State(vs, state))
            {
                cube->SetState(state);
                std::cout << "success" << std::endl;
            }
            else
            {
                std::cout << "fail" << std::endl;
            }
        }
        else if (cmd == "solve")
        {
            auto start = std::chrono::system_clock::now();

            auto str = cube->Solve();

            std::cout << str << std::endl;

            auto end = std::chrono::system_clock::now();

            std::chrono::duration<double> diff = end - start;

            std::cout << "Time used (in seconds) :" << diff.count() << std::endl;
        }
        else if (cmd == "random")
        {
            int n;
            std::cin >> n;

            std::string s = cube->Random(n);

            std::cout << s << std::endl;
        }
    }

    return 0;
}
