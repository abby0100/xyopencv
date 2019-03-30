#ifndef __TIME_UTIL_H__
#define __TIME_UTIL_H__

#include <chrono>
//#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

class TimeUtil {
public:
	TimeUtil();
	~TimeUtil();

	void StartCount();
	void StopCount(std::string func);
	void Report();

	static std::string to_string();

private:
	std::chrono::_V2::system_clock::time_point start_time;
	std::chrono::_V2::system_clock::time_point end_time;
	std::chrono::nanoseconds duration_time;

	std::vector<std::string> funcs;
	std::vector<float> funcs_times;
};

#endif
