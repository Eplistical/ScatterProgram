#ifndef _SCATTER_CME_HPP
#define _SCATTER_CME_HPP
// module for CME & bCME

#include <vector>
#include "simulation.hpp"

namespace scatter{
	namespace simulation{

		struct hop_t{
			size_t id;
			size_t from;
			size_t to;
			std::vector<double> r;
			std::vector<double> p;
			double gamma;
			double energy_gap;

			explicit hop_t(size_t ID, size_t From, size_t To, double EnergyGap, double Gamma, const std::vector<double>& R, const std::vector<double>& P)
				: id(ID), from(From), to(To), energy_gap(EnergyGap), gamma(Gamma), r(R), p(P) {}
			hop_t(const hop_t& other) = default;
			~hop_t() = default;
			hop_t& operator=(const hop_t& other) = default;
		};

		extern std::vector<hop_t> CME_hop_recorder;
		extern std::vector<hop_t> BCME_hop_recorder;

		void CME(particle_t& ptcl, size_t trajID);
		void BCME(particle_t& ptcl, size_t trajID);
	};
};

#endif
