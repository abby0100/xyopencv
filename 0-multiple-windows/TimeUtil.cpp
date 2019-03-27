#include "TimeUtil.hpp"

TimeUtil::TimeUtil() {

}

void TimeUtil::Report() {
	int size = funcs.size();

	std::cout << "--------------------------Count Time-------------------------------------" << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << " [-] " << std::setw(20) << funcs[i] << std::setw(8) << funcs_times[i] << " us" << std::endl;
		//std::cout << " [-] " << std::setw(20) << std::setfill(' ') << std::left << funcs[i] << "\t" << std::setw(8) << funcs_times[i] << " us" << std::endl;
	}
	std::cout << "-------------------------------------------------------------------------" << std::endl;
}

void TimeUtil::StartCount() {
	start_time = std::chrono::high_resolution_clock::now();
}

void TimeUtil::StopCount(std::string func) {
	end_time = std::chrono::high_resolution_clock::now();
	duration_time = end_time - start_time;
	float func_time = duration_time.count() / 1000;

	funcs.push_back(func);
	funcs_times.push_back(func_time);
}

std::string TimeUtil::to_string() {
	return "Hello from TimeUtil!";
}
