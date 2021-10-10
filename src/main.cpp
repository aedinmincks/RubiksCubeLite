// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "RubiksCube.h"
#include "config.h"

#include <cassert>
#include <ctime>
#include <regex>

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

    std::shared_ptr<CRubiksCube> mc = std::make_shared<CRubiksCube>(CConfig::size);

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
            mc->show();
        }
        else if (cmd == "do")
        {
            /*std::string s;
            std::cin >> s;

            std::regex r(CConfig::GetRegex(mc->level_));
            std::smatch sm;
            while (std::regex_search(s, sm, r))
            {
                std::cout << sm.str() << std::endl;

                auto &input = CConfig::InputsMap[mc->level_][sm.str()];

                assert(input.axis >= (int)EAxis::x && input.axis <= (int)EAxis::z);
                assert(input.angle >= (int)EAngle::_0 && input.angle <= (int)EAngle::_270);

                mc->Rotate((EAxis)input.axis, input.start, input.end, (EAngle)input.angle);

                s = sm.suffix();
            }*/
        }
        else if (cmd == "get")
        {
            CCubeLogic::PrintFacelets(mc->GetFacelets());
        }
        else if (cmd == "set")
        {
            /*std::string s;
            std::cin >> s;

            if (!CCubeLogic::checkColors(s, mc->level_))
            {
                std::cout << "Input format error!\n";
                continue;
            }

            std::cout << "old\n" << std::endl;

            mc->show();

            mc->SetColors(s);

            std::cout << "new\n" << std::endl;

            mc->show();

            std::cout << "done\n" << std::endl;*/
        }
        else if (cmd == "rec")
        {
            /*time_t start = time(0);

            std::cout << CCubeLogic::FindShortestPath(mc->GetColors(), CConfig::SourceColorsMap[mc->level_], mc->level_)
                      << std::endl;

            time_t end = time(0);

            std::cout << "Time used (in seconds) :" << end - start << std::endl;*/
        }
        else if (cmd == "random")
        {
            int n;
            std::cin >> n;

            std::string s = mc->RandomRotate(n);

            std::cout << s << std::endl;

            mc->show();
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
