#ifndef _SCATTER_SIMULATION_VAR_HPP
#define _SCATTER_SIMULATION_VAR_HPP
// module for variables in simulation
#include "types.hpp"

#include <vector>
#include "json_toolkit.hpp"
#include "initmode.hpp"

namespace scatter{
	namespace simulation{
		// simulation control parameters
		extern std::vector<DOUBLE_T> mass;
		extern std::vector<DOUBLE_T> omega;
		extern UINT_T Ntraj;
		extern UINT_T Nstep;
		extern DOUBLE_T EndT;
		extern DOUBLE_T dt;
		extern UINT_T Anastep;

		// for init state
		extern BOOL_T prepinit;
		extern DOUBLE_T inittemp;
		extern UINT_T vibstate;
		extern UINT_T elestate;
		extern std::vector<enumspace::initmode_enum> initmode;
		extern std::vector<DOUBLE_T> initravg;
		extern std::vector<DOUBLE_T> initpavg;
		extern std::vector<DOUBLE_T> r0p0;

		void load_var(const rapidjson::Document& doc);
		void print_var(void);
	};
};

#endif
