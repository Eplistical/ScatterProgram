#ifndef _TIMER_HPP
#define _TIMER_HPP
/* module for timer 
 * require C++11
 * written based on <chrono> and <ctime>
 *
 * Gaohan
 */
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <unordered_map>

namespace timer{
	using namespace std::chrono;
	using TicTimeType = std::unordered_map<int, time_point<system_clock> >;

	static TicTimeType tic_time;

	inline void tic(int index = 0) noexcept{
		tic_time[index] = system_clock::now();
	}

	inline std::string toc(int index = 0) noexcept{
		if (tic_time.find(index) == tic_time.end()) {
			return std::string("timer::toc: error!, index not found");
		}
		duration<double> elapsed = system_clock::now() - tic_time[index];
		std::ostringstream buf;
		buf << "Time elapsed: " << elapsed.count() << " s" ;
		return buf.str();
	}

	inline std::string now(void) noexcept{
		const time_t _now = system_clock::to_time_t(system_clock::now());
		std::ostringstream buf;
		buf << "now: " << ctime(&_now);
		return buf.str();
	}
};
#endif
