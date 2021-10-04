#include "config.h"
#include <iostream>
#include <fstream>

void CConfig::Load(std::filesystem::path p)
{
	p /= std::filesystem::path("config.yaml");

	std::string filename = std::filesystem::absolute(p).string();

	printf("load config file <%s> \n", filename.c_str());

	std::ifstream istrm(filename, std::ios::binary | std::ios::ate);
	if (istrm.is_open()) {
		auto size = istrm.tellg();
        std::string str(size, '\0'); // construct string to stream size
        istrm.seekg(0);
		if (istrm.read(&str[0], size)) {
			
		}
		else {
			std::cout << "failed to read " << filename << '\n';
		}
	}
	else {
		std::cout << "failed to open " << filename << '\n';
	}
}
