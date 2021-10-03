// MagicCubeLite.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>

/*
*	  z
*	  |
*	  |___y
*	 /
*	/x
*/

enum class EAxis : uint8_t {
	pitch = 0,
	yaw,
	roll,
};

enum class EDirection : uint8_t {
	up = 0,
	down,
	left,
	right,
	front,
	back,
};

enum class EAngle : uint8_t {
	_0 = 0,
	_90,
	_180,
	_270,
};

const static std::vector<char> Dir2ColorMap{'W', 'Y', 'G', 'B', 'R', 'O'};

const static std::vector<std::vector<std::vector<int>>> surface_table{
	//up
	{
		//pitch
		{
			(int)EDirection::up,
			(int)EDirection::left,
			(int)EDirection::down,
			(int)EDirection::right,
		},
		//yaw
		{
			(int)EDirection::up,
			(int)EDirection::front,
			(int)EDirection::down,
			(int)EDirection::back,
		},
		//roll
		{
			(int)EDirection::up,
			(int)EDirection::up,
			(int)EDirection::up,
			(int)EDirection::up,
		},
	},
	//down
	{
		//pitch
		{
			(int)EDirection::down,
			(int)EDirection::right,
			(int)EDirection::up,
			(int)EDirection::left,
		},
		//yaw
		{
			(int)EDirection::down,
			(int)EDirection::back,
			(int)EDirection::up,
			(int)EDirection::front,
		},
		//roll
		{
			(int)EDirection::down,
			(int)EDirection::down,
			(int)EDirection::down,
			(int)EDirection::down,
		},
	},
	//left
	{
		//pitch
		{
			(int)EDirection::left,
			(int)EDirection::down,
			(int)EDirection::right,
			(int)EDirection::up,
		},
		//yaw
		{
			(int)EDirection::left,
			(int)EDirection::left,
			(int)EDirection::left,
			(int)EDirection::left,
		},
		//roll
		{
			(int)EDirection::left,
			(int)EDirection::front,
			(int)EDirection::right,
			(int)EDirection::back,
		},
	},
	//right
	{
		//pitch
		{
			(int)EDirection::right,
			(int)EDirection::up,
			(int)EDirection::left,
			(int)EDirection::down,
		},
		//yaw
		{
			(int)EDirection::right,
			(int)EDirection::right,
			(int)EDirection::right,
			(int)EDirection::right,
		},
		//roll
		{
			(int)EDirection::right,
			(int)EDirection::back,
			(int)EDirection::left,
			(int)EDirection::front,
		},
	},
	//front
	{
		//pitch
		{
			(int)EDirection::front,
			(int)EDirection::front,
			(int)EDirection::front,
			(int)EDirection::front,
		},
		//yaw
		{
			(int)EDirection::front,
			(int)EDirection::down,
			(int)EDirection::back,
			(int)EDirection::up,
		},
		//roll
		{
			(int)EDirection::front,
			(int)EDirection::right,
			(int)EDirection::back,
			(int)EDirection::left,
		},
	},
	//back
	{
		//pitch
		{
			(int)EDirection::back,
			(int)EDirection::back,
			(int)EDirection::back,
			(int)EDirection::back,
		},
		//yaw
		{
			(int)EDirection::back,
			(int)EDirection::up,
			(int)EDirection::front,
			(int)EDirection::down,
		},
		//roll
		{
			(int)EDirection::back,
			(int)EDirection::left,
			(int)EDirection::front,
			(int)EDirection::right,
		},
	},
};

class CMagicCube {
public:
	int level_;
	std::vector<std::vector<std::vector<char>>>	colors_;

public:
	CMagicCube(int level) : level_(level) {
		colors_.assign(6, std::vector<std::vector<char>>(level, std::vector<char>(level, '.')));
		for (int i = (int)EDirection::up; i <= (int)EDirection::back; i++) {
			for (int j = 0; j < level; j++) {
				for (int k = 0; k < level; k++) {
					colors_[i][j][k] = Dir2ColorMap[i];
				}
			}
		}
	}

	void Rotate(EAxis axis, int start, int end, EAngle angle) {
		if (start < 0 || start >= level_ || end < 0 || end >= level_) {
			
			return;
		}

		std::vector<std::vector<std::vector<char>>>	copy(colors_);
		
		
	}

	void print() {
		int n = level_;

		std::cout << std::endl;

		for (int i = 0; i < n; i++) {
			std::cout << std::string(n, ' ');
			for (int j = 0; j < n; j++) {
				std::cout << colors_[(uint8_t)EDirection::back][i][j];
			}
			std::cout << std::endl;
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				std::cout << colors_[(uint8_t)EDirection::left][i][j];
			}
			for (int j = 0; j < n; j++) {
				std::cout << colors_[(uint8_t)EDirection::up][i][j];
			}
			for (int j = 0; j < n; j++) {
				std::cout << colors_[(uint8_t)EDirection::right][i][j];
			}
			for (int j = 0; j < n; j++) {
				std::cout << colors_[(uint8_t)EDirection::down][i][j];
			}
			std::cout << std::endl;
		}

		for (int i = 0; i < n; i++) {
			std::cout << std::string(n, ' ');
			for (int j = 0; j < n; j++) {
				std::cout << colors_[(uint8_t)EDirection::front][i][j];
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

public:
	static std::string Serialization(std::vector<std::vector<std::vector<char>>>& colors, int level) {

	}

	static std::vector<std::vector<std::vector<char>>> Deserialization(std::string s) {

	}

};

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

			if (level >= 2 && level <= 3) {
				mc = std::make_shared<CMagicCube>(level);

				std::cout << "level initialize done!\n";
			}
			else {
				printf("The level<%d> should be in the [2, 3] interval.\n", level);
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
