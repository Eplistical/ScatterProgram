#ifndef _RANDOMER_HPP
#define _RANDOMER_HPP
/* module for random
 * require C++11
 * written based on <random> and <vector>
 *
 * Gaohan
 */
#include <cmath>
#include <random>
#include <vector>

namespace randomer{
	static std::random_device ran_dev;
	static std::mt19937 gen(ran_dev());

	inline double rand(double lb = 0.0, double ub = 1.0) noexcept{
		std::uniform_real_distribution<> dis(lb, ub);
		return dis(gen);
	}

	inline std::vector<double> vrand(size_t N, double lb = 0.0, double ub = 1.0) noexcept{
		std::uniform_real_distribution<> dis(lb, ub);
		std::vector<double> rst(N);
		for(size_t j = 0; j < N; ++j)
			rst[j] = dis(gen);
		return rst;
	}

	inline double normrand(double mu = 0.0, double sigma = 1.0) noexcept{
		std::normal_distribution<double> dis(mu, sigma);
		return dis(gen);
	}

	inline std::vector<double> vnormrand(size_t N, double mu = 0.0, double sigma = 1.0) noexcept{
		std::normal_distribution<double> dis(mu, sigma);
		std::vector<double> rst(N);
		for(size_t j = 0; j < N; ++j)
			rst[j] = dis(gen);
		return rst;
	}
};
#endif
