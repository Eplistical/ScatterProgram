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

namespace timer{
	using namespace std::chrono;

	static time_point<system_clock> tic_time;

	inline void tic(void) noexcept{
		tic_time = system_clock::now();
	}

	inline void toc(void) noexcept{
		duration<double> elapsed = system_clock::now() - tic_time;
		std::cout 
			<< "Time elapsed: " << elapsed.count() << " s"
			<< std::endl;
	}

	inline void now(void) noexcept{
		const time_t _now = system_clock::to_time_t(system_clock::now());
		std::cout 
			<< "now: " << ctime(&_now)
			<< std::endl;
	}
};
#endif
