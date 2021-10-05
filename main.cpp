// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "MagicCube.h"
#include "config.h"

#include <regex>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("argc<%d> is less than 2", argc);
        return 1;
    }

    CConfig::Load(std::filesystem::path(argv[1]));

    std::shared_ptr<CMagicCube> mc;

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
        else if (cmd == "init")
        {
            int level = 0;
            std::cin >> level;

            if (level >= 2 && level <= 10)
            {
                mc = std::make_shared<CMagicCube>(level);

                std::cout << "level initialize done!\n";
            }
            else
            {
                printf("The level<%d> should be in the [2, 10] interval.\n", level);
            }
        }
        else if (cmd == "print")
        {
            if (mc == nullptr)
            {
                std::cout << "please initialize first!\n";
                continue;
            }

            mc->print();
        }
        else if (cmd == "do")
        {
            if (mc == nullptr)
            {
                std::cout << "please initialize first!\n";
                continue;
            }

            std::string s;
            std::cin >> s;

            std::regex r(CConfig::GetRegex(mc->level_));
            std::smatch sm;
            while (std::regex_search(s, sm, r))
            {
                std::cout << sm.str() << std::endl;

                auto &input = CConfig::InputsMap[mc->level_][sm.str()];

                mc->Rotate((EAxis)input.axis, input.start, input.end, (EAngle)input.angle);

                s = sm.suffix();
            }
        }
        else if (cmd == "get")
        {
            if (mc == nullptr)
            {
                std::cout << "please initialize first!\n";
                continue;
            }

            std::cout << CCodec::Serialization(mc->GetColors()) << std::endl;
        }
        else if (cmd == "set")
        {
            if (mc == nullptr)
            {
                std::cout << "please initialize first!\n";
                continue;
            }

            std::string s;
            std::cin >> s;

            if (!CCodec::checkColors(s, mc->level_))
            {
                std::cout << "Input format error!\n";
                continue;
            }

            std::cout << "old\n" << std::endl;

            mc->print();

            auto c = CCodec::Deserialization(s);

            mc->SetColors(c);

            std::cout << "new\n" << std::endl;

            mc->print();

            std::cout << "done\n" << std::endl;
        }
        else if (cmd == "rec")
        {
            if (mc == nullptr)
            {
                std::cout << "please initialize first!\n";
                continue;
            }
            // to do
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
