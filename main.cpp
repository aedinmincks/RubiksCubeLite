// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "MagicCube.h"

int main()
{
	int level = 0;
	std::shared_ptr<CMagicCube> mc;

	while (1) {
		std::string cmd;

		std::cout << "#";
		std::cin >> cmd;

		if (cmd == "exit" || cmd == "quit") {
			std::cout << "Good bye!\n";
			break;
		}
		else if (cmd == "help")
		{
			std::cout << "just see code.\n";
		}
		else if (cmd == "init") {
			std::cin >> level;

			if (level >= 1 && level <= 10) {
				mc = std::make_shared<CMagicCube>(level);

				std::cout << "level initialize done!\n";
			}
			else {
				printf("The level<%d> should be in the [1, 10] interval.\n", level);
			}
		}
		else if (cmd == "print")
		{
			if (mc == nullptr) {
				std::cout << "please initialize first!\n";
				continue;
			}

			mc->print();
		}
		else if (cmd == "do")
		{
			if (mc == nullptr) {
				std::cout << "please initialize first!\n";
				continue;
			}

			std::string s;
			std::cin >> s;

			for (auto c : s) {
				switch (c)
				{
				case 'R':
					mc->Rotate(EAxis::y, level - 1, level - 1, EAngle::_90);
					break;
				case 'L':
					mc->Rotate(EAxis::y, 0, 0, EAngle::_270);
					break;
				case 'F':
					mc->Rotate(EAxis::x, level - 1, level - 1, EAngle::_90);
					break;
				case 'B':
					mc->Rotate(EAxis::x, 0, 0, EAngle::_270);
					break;
				case 'U':
					mc->Rotate(EAxis::z, level - 1, level - 1, EAngle::_90);
					break;
				case 'D':
					mc->Rotate(EAxis::z, 0, 0, EAngle::_270);
					break;
				case 'r':
					mc->Rotate(EAxis::y, level - 1, level - 1, EAngle::_270);
					break;
				case 'l':
					mc->Rotate(EAxis::y, 0, 0, EAngle::_90);
					break;
				case 'f':
					mc->Rotate(EAxis::x, level - 1, level - 1, EAngle::_270);
					break;
				case 'b':
					mc->Rotate(EAxis::x, 0, 0, EAngle::_90);
					break;
				case 'u':
					mc->Rotate(EAxis::z, level - 1, level - 1, EAngle::_270);
					break;
				case 'd':
					mc->Rotate(EAxis::z, 0, 0, EAngle::_90);
					break;
				default:
					break;
				}
			}
		}
		else if (cmd == "serail") {
			if (mc == nullptr) {
				std::cout << "please initialize first!\n";
				continue;
			}

			//std::cout << CMagicCube::Serialization(mc->cubes_, mc->level_);
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
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
