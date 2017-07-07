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

namespace timer{
	using namespace std::chrono;

	static time_point<system_clock> tic_time;

	inline void tic(void) noexcept{
		tic_time = system_clock::now();
	}

	inline std::string toc(void) noexcept{
		duration<double> elapsed = system_clock::now() - tic_time;
		std::ostringstream buf;
		buf 
			<< "Time elapsed: " << elapsed.count() << " s"
			<< "\n";
		return buf.str();
	}

	inline std::string now(void) noexcept{
		const time_t _now = system_clock::to_time_t(system_clock::now());
		std::ostringstream buf;
		buf 
			<< "now: " << ctime(&_now)
			<< "\n";
		return buf.str();
	}
};
#endif
