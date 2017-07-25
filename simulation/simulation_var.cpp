#include "scatter_basic.hpp"
#include <vector>
#include "rem.hpp"
#include "io.hpp"
#include "string_mode_conversion.hpp"
#include "surfaces.hpp"
#include "simulation_var.hpp"

using scatter::io::out_handler;

// variables in scatter::simulation
namespace NS = scatter::simulation;
using enumspace::initmode_enum;
using enumspace::dynamics_mode_enum;

std::vector<DOUBLE_T> NS::mass;
std::vector<DOUBLE_T> NS::omega;
UINT_T NS::Ntraj;
UINT_T NS::Nstep;
DOUBLE_T NS::EndT;
DOUBLE_T NS::dt;
UINT_T NS::Anastep;
std::vector<dynamics_mode_enum> NS::algorithms;

// for init state
DOUBLE_T NS::inittemp;
UINT_T NS::vibstate;
UINT_T NS::elestate;
std::vector<DOUBLE_T> NS::initravg;
std::vector<DOUBLE_T> NS::initpavg;

std::vector<initmode_enum> NS::initmode;
std::vector<DOUBLE_T> NS::r0p0;


// helper function
static VOID_T extract_omega(VOID_T){
	// extract omega for all harmonic surfaces
	using namespace scatter;
	std::vector<DOUBLE_T> tmp;
	for(UINT_T i = 0; i < surfaces::surfnum; ++i){
		for(UINT_T d = 0; d < rem::dim; ++d){
			if(surfaces::surfmode.at(d + i * rem::dim) == enumspace::surfmode_enum::HARMONIC){
				tmp.push_back( pow(surfaces::surfpara.at(d + i * rem::dim).at(0) / simulation::mass.at(d), 0.5) );
			}
		}
	}
	scatter::simulation::omega = tmp;
}

// init simulation parameters 
VOID_T scatter::simulation::load_var(const rapidjson::Document& doc){
	using namespace scatter::simulation;
	using json::load_param;
	load_param(doc, "simulation")
		("Ntraj", Ntraj)
		("mass", mass)
		("EndT", EndT)
		("Nstep", Nstep)
		("Anastep", Anastep)
		("inittemp", inittemp)
		("initravg", initravg)
		("initpavg", initpavg)
		("vibstate", vibstate)
		("elestate", elestate)
		("initmode", initmode, enumspace::initmode_dict)
		("algorithms", algorithms, enumspace::dynamics_mode_dict)
		;
	dt = EndT / Nstep;
	// init omega
	extract_omega();
}

// prINT_T out simulation parameters
VOID_T scatter::simulation::print_var(VOID_T){
	using namespace scatter::simulation;
	out_handler.info("simulation info");
	out_handler.drawline('-');
	out_handler.keyval()
		("Ntraj", Ntraj)
		("mass", mass)
		("omega", omega)
		("EndT", EndT)
		("Nstep", Nstep)
		("dt", dt)
		("Anastep", Anastep)
		("inittemp", inittemp)
		("initravg", initravg)
		("initpavg", initpavg)
		("vibstate", vibstate)
		("elestate", elestate)
		("initmode", enumspace::mode_to_string(initmode, enumspace::initmode_dict))
		("algorithms", enumspace::mode_to_string(algorithms, enumspace::dynamics_mode_dict))
		;
	out_handler.drawline('-');
}
