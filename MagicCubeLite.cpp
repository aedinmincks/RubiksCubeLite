// MagicCubeLite.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "fmt/core.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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

enum class ESurface : uint8_t {
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

const static std::vector<std::vector<std::vector<ESurface>>> surface_table{
	//up
	{
		//pitch
		{
			ESurface::up,
			ESurface::left,
			ESurface::down,
			ESurface::right,
		},
		//yaw
		{
			ESurface::up,
			ESurface::front,
			ESurface::down,
			ESurface::back,
		},
		//roll
		{
			ESurface::up,
			ESurface::up,
			ESurface::up,
			ESurface::up,
		},
	},
	//down
	{
		//pitch
		{
			ESurface::down,
			ESurface::right,
			ESurface::up,
			ESurface::left,
		},
		//yaw
		{
			ESurface::down,
			ESurface::back,
			ESurface::up,
			ESurface::front,
		},
		//roll
		{
			ESurface::down,
			ESurface::down,
			ESurface::down,
			ESurface::down,
		},
	},
	//left
	{
		//pitch
		{
			ESurface::left,
			ESurface::down,
			ESurface::right,
			ESurface::up,
		},
		//yaw
		{
			ESurface::left,
			ESurface::left,
			ESurface::left,
			ESurface::left,
		},
		//roll
		{
			ESurface::left,
			ESurface::front,
			ESurface::right,
			ESurface::back,
		},
	},
	//right
	{
		//pitch
		{
			ESurface::right,
			ESurface::up,
			ESurface::left,
			ESurface::down,
		},
		//yaw
		{
			ESurface::right,
			ESurface::right,
			ESurface::right,
			ESurface::right,
		},
		//roll
		{
			ESurface::right,
			ESurface::back,
			ESurface::left,
			ESurface::front,
		},
	},
	//front
	{
		//pitch
		{
			ESurface::front,
			ESurface::front,
			ESurface::front,
			ESurface::front,
		},
		//yaw
		{
			ESurface::front,
			ESurface::down,
			ESurface::back,
			ESurface::up,
		},
		//roll
		{
			ESurface::front,
			ESurface::right,
			ESurface::back,
			ESurface::left,
		},
	},
	//back
	{
		//pitch
		{
			ESurface::back,
			ESurface::back,
			ESurface::back,
			ESurface::back,
		},
		//yaw
		{
			ESurface::back,
			ESurface::up,
			ESurface::front,
			ESurface::down,
		},
		//roll
		{
			ESurface::back,
			ESurface::left,
			ESurface::front,
			ESurface::right,
		},
	},
};

class CCube {
private:
	int level_;
	int x_, y_, z_;
	std::string surfaces_;

	void rotate(int& x, int& y, int n, EAngle angle) {
		int t;

		switch (angle)
		{
		case EAngle::_0:
			break;
		case EAngle::_90:
			t = y;
			y = n - 1 - x;
			x = t;
			break;
		case EAngle::_180:
			y = n - 1 - y;
			x = n - 1 - x;
			break;
		case EAngle::_270:
			t = y;
			y = x;
			x = n - 1 - t;
			break;
		default:
			break;
		}
	}

	void rotato_surface(EAxis axis, EAngle angle) {
		std::string copy(surfaces_);
		uint8_t b;
		b = (uint8_t)surface_table[(uint8_t)ESurface::up][(uint8_t)axis][(uint8_t)angle];
		surfaces_[(uint8_t)ESurface::up] = copy[b];
		b = (uint8_t)surface_table[(uint8_t)ESurface::down][(uint8_t)axis][(uint8_t)angle];
		surfaces_[(uint8_t)ESurface::down] = copy[b];
		b = (uint8_t)surface_table[(uint8_t)ESurface::left][(uint8_t)axis][(uint8_t)angle];
		surfaces_[(uint8_t)ESurface::left] = copy[b];
		b = (uint8_t)surface_table[(uint8_t)ESurface::right][(uint8_t)axis][(uint8_t)angle];
		surfaces_[(uint8_t)ESurface::right] = copy[b];
		b = (uint8_t)surface_table[(uint8_t)ESurface::front][(uint8_t)axis][(uint8_t)angle];
		surfaces_[(uint8_t)ESurface::front] = copy[b];
		b = (uint8_t)surface_table[(uint8_t)ESurface::back][(uint8_t)axis][(uint8_t)angle];
		surfaces_[(uint8_t)ESurface::back] = copy[b];
	}

public:
	CCube(int x, int y, int z, int level) :
		level_(level), x_(x), y_(y), z_(z) {
		surfaces_.assign(6, ' ');
		surfaces_[(uint8_t)ESurface::up] = (z_ == level_ - 1) ? 'w' : '.';
		surfaces_[(uint8_t)ESurface::down] = (z_ == 0) ? 'y' : '.';
		surfaces_[(uint8_t)ESurface::left] = (y_ == 0) ? 'g' : '.';
		surfaces_[(uint8_t)ESurface::right] = (y_ == level_ - 1) ? 'b' : '.';
		surfaces_[(uint8_t)ESurface::front] = (x_ == level_ - 1) ? 'r' : '.';
		surfaces_[(uint8_t)ESurface::back] = (x_ == 0) ? 'o' : '.';
	}

	void Rotate(EAxis axis, EAngle angle) {
		switch (axis) {
		case EAxis::pitch:
			rotate(y_, z_, level_, angle);
			rotato_surface(axis, angle);
			break;
		case EAxis::yaw:
			rotate(z_, x_, level_, angle);
			rotato_surface(axis, angle);
			break;
		case EAxis::roll:
			rotate(x_, y_, level_, angle);
			rotato_surface(axis, angle);
			break;
		default:
			break;
		}
	}

	int GetX() {
		return x_;
	}

	int GetY() {
		return y_;
	}

	int GetZ() {
		return z_;
	}

	int GetValueByAxis(EAxis axis) {
		int ans = -1;
		switch (axis)
		{
		case EAxis::pitch:
			ans = x_;
			break;
		case EAxis::yaw:
			ans = y_;
			break;
		case EAxis::roll:
			ans = z_;
			break;
		default:
			break;
		}

		return ans;
	}

	char GetTop() {
		return surfaces_[(uint8_t)ESurface::up];
	}

	char GetBottom() {
		return surfaces_[(uint8_t)ESurface::down];
	}

	char GetLeft() {
		return surfaces_[(uint8_t)ESurface::left];
	}

	char GetRight() {
		return surfaces_[(uint8_t)ESurface::right];
	}

	char GetFront() {
		return surfaces_[(uint8_t)ESurface::front];
	}

	char GetBack() {
		return surfaces_[(uint8_t)ESurface::back];
	}

	void SetSurfaces(std::string& v) {
		if (v.size() != 6) {
			return;
		}

		surfaces_ = v;
	}

	std::string GetSurfaces() {
		return surfaces_;
	}
};


class CMagicCube {
public:
	int level_;
	std::vector<std::shared_ptr<CCube>> cubes_;

public:
	CMagicCube(int level) : level_(level) {
		cubes_.assign(level * level * level, nullptr);
		for (int i = 0; i < level; i++) {
			for (int j = 0; j < level; j++) {
				for (int k = 0; k < level; k++) {
					cubes_[i * level * level + j * level + k] = std::make_shared<CCube>(i, j, k, level);
				}
			}
		}
	}

	void Rotate(EAxis axis, int index, EAngle angle) {
		for (auto cube : cubes_) {
			int i = cube->GetValueByAxis(axis);
			if (i == index) {
				cube->Rotate(axis, angle);
			}
		}
	}

	void print() {
		int n = level_;
		std::vector<std::vector<std::vector<char>>>	colors(6, std::vector<std::vector<char>>(n, std::vector<char>(n, ' ')));


		for (auto cube : cubes_) {
			int x = cube->GetX();
			int y = cube->GetY();
			int z = cube->GetZ();

			if (x == 0) {
				colors[(uint8_t)ESurface::back][z][y] = cube->GetBack();
			}
			else if (x == n - 1) {
				colors[(uint8_t)ESurface::front][n - 1 - z][y] = cube->GetFront();
			}
			if (y == 0) {
				colors[(uint8_t)ESurface::left][x][z] = cube->GetLeft();
			}
			else if (y == n - 1) {
				colors[(uint8_t)ESurface::right][x][n - 1 - z] = cube->GetRight();
			}
			if (z == 0) {
				colors[(uint8_t)ESurface::down][x][n - 1 - y] = cube->GetBottom();
			}
			else if (z == n - 1) {
				colors[(uint8_t)ESurface::up][x][y] = cube->GetTop();
			}
		}

		std::cout << std::endl;

		for (int i = 0; i < n; i++) {
			std::cout << std::string(n, ' ');
			for (int j = 0; j < n; j++) {
				std::cout << colors[(uint8_t)ESurface::back][i][j];
			}
			std::cout << std::endl;
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				std::cout << colors[(uint8_t)ESurface::left][i][j];
			}
			for (int j = 0; j < n; j++) {
				std::cout << colors[(uint8_t)ESurface::up][i][j];
			}
			for (int j = 0; j < n; j++) {
				std::cout << colors[(uint8_t)ESurface::right][i][j];
			}
			for (int j = 0; j < n; j++) {
				std::cout << colors[(uint8_t)ESurface::down][i][j];
			}
			std::cout << std::endl;
		}

		for (int i = 0; i < n; i++) {
			std::cout << std::string(n, ' ');
			for (int j = 0; j < n; j++) {
				std::cout << colors[(uint8_t)ESurface::front][i][j];
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

public:
	static std::string Serialization(std::vector<std::shared_ptr<CCube>>& cubes, int level) {
		rapidjson::Document root;
		root.SetArray();
		auto& allocator = root.GetAllocator();

		for (int n = 0; n < cubes.size(); n++) {
			auto cube = cubes[n];

			int x = cube->GetX();
			int y = cube->GetY();
			int z = cube->GetZ();
			int m = x * level * level + y * level + z;

			std::string s = cube->GetSurfaces();

			rapidjson::Value obj(rapidjson::kObjectType);

			obj.AddMember("n", n, allocator);
			obj.AddMember("m", m, allocator);
			obj.AddMember("s", rapidjson::Value().SetString(s.c_str(), allocator), allocator);

			root.PushBack(obj, allocator);
		}

		rapidjson::StringBuffer strBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strBuffer);
		root.Accept(writer);

		return strBuffer.GetString();
	}

	static std::vector<std::shared_ptr<CCube>> Deserialization(std::string s) {

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
				std::cout << fmt::format("The level<{}> should be in the [2, 3] interval.\n", level);
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
				case 'u':
					mc->Rotate(EAxis::roll, level - 1, EAngle::_270);
					break;
				case 'U':
					mc->Rotate(EAxis::roll, level - 1, EAngle::_90);
					break;
				case 'd':
					mc->Rotate(EAxis::roll, 0, EAngle::_90);
					break;
				case 'D':
					mc->Rotate(EAxis::roll, 0, EAngle::_270);
					break;
				case 'l':
					mc->Rotate(EAxis::yaw, 0, EAngle::_90);
					break;
				case 'L':
					mc->Rotate(EAxis::yaw, 0, EAngle::_270);
					break;
				case 'r':
					mc->Rotate(EAxis::yaw, level - 1, EAngle::_270);
					break;
				case 'R':
					mc->Rotate(EAxis::yaw, level - 1, EAngle::_90);
					break;
				case 'f':
					mc->Rotate(EAxis::pitch, level - 1, EAngle::_270);
					break;
				case 'F':
					mc->Rotate(EAxis::pitch, level - 1, EAngle::_90);
					break;
				case 'b':
					mc->Rotate(EAxis::pitch, 0, EAngle::_90);
					break;
				case 'B':
					mc->Rotate(EAxis::pitch, 0, EAngle::_270);
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

			std::cout << CMagicCube::Serialization(mc->cubes_, mc->level_);
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
