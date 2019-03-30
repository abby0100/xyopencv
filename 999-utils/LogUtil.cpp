#include "LogUtil.hpp"
#include <iostream>

//LogUtil::LogUtil() {}
//LogUtil::~LogUtil() {}

void LogUtil::d(std::string func, std::string info) {
	std::cout << func << " " << info << std::endl;
}
