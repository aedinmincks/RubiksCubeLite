// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "RubiksCube.h"
#include "config.h"

#include <cassert>
#include <regex>
#include <chrono>


#define MAXLEVEL (3)

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("argc<%d> is less than 2", argc);
        return 1;
    }

    if (!CConfig::Load(std::filesystem::path(argv[1])))
    {
        printf("yaml load error");
        return 1;
    }

    CRubiksCube mc(CConfig::size);

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
            mc.show();
        }
        else if (cmd == "do")
        {
            std::string s;
            std::cin >> s;

            std::regex r(CConfig::GetRegex());
            std::smatch sm;
            while (std::regex_search(s, sm, r))
            {
                std::string key = sm.str();

                std::cout << key << std::endl;

                mc.Rotate(key);

                s = sm.suffix();
            }
        }
        else if (cmd == "get")
        {
            CCubeLogic::PrintFacelets(mc.GetFacelets());
        }
        else if (cmd == "set")
        {
            std::string s;
            std::cin >> s;

            std::vector<int> v;

            std::regex r("([0-9]+)");
            std::smatch sm;
            while (std::regex_search(s, sm, r))
            {
                // std::cout << sm.str() << std::endl;

                v.emplace_back(std::stoi(sm.str()));

                s = sm.suffix();
            }

            if (mc.SetFacelets(v))
            {
                std::cout << "done!" << std::endl;
            }
            else
            {
                std::cout << "error!" << std::endl;
            }
        }
        else if (cmd == "solve")
        {
            auto start = std::chrono::system_clock::now();

            auto str = mc.Solve();

            std::cout << str << std::endl;

            auto end = std::chrono::system_clock::now();

            std::chrono::duration<double> diff = end - start;

            std::cout << "Time used (in seconds) :" << diff.count() << std::endl;
        }
        else if (cmd == "random")
        {
            int n;
            std::cin >> n;

            std::string s = mc.RandomRotate(n);

            std::cout << s << std::endl;

            mc.show();
        }
    }

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5.
//   转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
