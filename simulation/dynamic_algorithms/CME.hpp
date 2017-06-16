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
			double energy_gap;

			explicit hop_t(size_t ID, size_t From, size_t To, double EnergyGap) :
				id(ID), from(From), to(To), energy_gap(EnergyGap) {} 
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
