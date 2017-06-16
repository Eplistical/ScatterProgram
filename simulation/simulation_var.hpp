#ifndef _SCATTER_SIMULATION_VAR_HPP
#define _SCATTER_SIMULATION_VAR_HPP
// module for variables in simulation

#include <string>
#include <vector>
#include "json_toolkit.hpp"
#include "initmode.hpp"

namespace scatter{
	namespace simulation{
		// simulation control parameters
		extern const std::vector<double>& mass;
		extern const std::vector<double>& omega;
		extern const size_t& Ntraj;
		extern const size_t& Nstep;
		extern const double& EndT;
		extern const double& dt;
		extern const size_t& Anastep;

		// for init state
		extern const bool& prepinit;
		extern const double& inittemp;
		extern const size_t& vibstate;
		extern const size_t& elestate;
		extern const std::vector<enumspace::initmode_enum>& initmode;
		extern const std::vector<double>& initravg;
		extern const std::vector<double>& initpavg;
		extern const std::vector<double>& r0p0;

		void load_var(const rapidjson::Document& doc);
		void print_var(void);
	};
};

#endif
