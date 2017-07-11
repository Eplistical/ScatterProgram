#ifndef _SCATTER_SIMULATION_VAR_HPP
#define _SCATTER_SIMULATION_VAR_HPP
// module for variables in simulation
#include "types.hpp"

#include <string>
#include <vector>
#include "json_toolkit.hpp"
#include "initmode.hpp"

namespace scatter{
	namespace simulation{
		// simulation control parameters
		extern std::vector<double> mass;
		extern std::vector<double> omega;
		extern size_t Ntraj;
		extern size_t Nstep;
		extern double EndT;
		extern double dt;
		extern size_t Anastep;

		// for init state
		extern bool prepinit;
		extern double inittemp;
		extern size_t vibstate;
		extern size_t elestate;
		extern std::vector<enumspace::initmode_enum> initmode;
		extern std::vector<double> initravg;
		extern std::vector<double> initpavg;
		extern std::vector<double> r0p0;

		void load_var(const rapidjson::Document& doc);
		void print_var(void);
	};
};

#endif
