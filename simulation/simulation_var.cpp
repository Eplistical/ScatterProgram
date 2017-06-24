#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <string>
#include <vector>
#include "rem.hpp"
#include "ioer.hpp"
#include "string_mode_conversion.hpp"
#include "surfaces.hpp"
#include "simulation_var.hpp"

// variables in scatter::simulation
namespace NS = scatter::simulation;
using enumspace::initmode_enum;

const std::vector<double>& NS::mass = std::vector<double>();
const std::vector<double>& NS::omega = std::vector<double>();
const size_t& NS::Ntraj = -1;
const size_t& NS::Nstep = -1;
const double& NS::EndT = -1;
const double& NS::dt = -1;
const size_t& NS::Anastep = 100;

// for init state
const bool& NS::prepinit = false;
const double& NS::inittemp = 1.0;
const size_t& NS::vibstate = 0;
const size_t& NS::elestate = 0;
const std::vector<double>& NS::initravg = std::vector<double>{0, 0};
const std::vector<double>& NS::initpavg = std::vector<double>{0, 0};

const std::vector<initmode_enum>& NS::initmode = std::vector<initmode_enum>();

const std::vector<double>& NS::r0p0 = std::vector<double>();


// helper function
static void extract_omega(void){
	// extract omega for all harmonic surfaces
	using namespace scatter;
	std::vector<double> tmp;
	for(size_t i = 0; i < surfaces::surfnum; ++i){
		for(size_t d = 0; d < rem::dim; ++d){
			if(surfaces::surfmode.at(d + i * rem::dim) == enumspace::surfmode_enum::HARMONIC){
				tmp.push_back( pow(surfaces::surfpara.at(d + i * rem::dim).at(0) / simulation::mass.at(d), 0.5) );
			}
		}
	}
	const_cast<std::vector<double>&>(scatter::simulation::omega) = tmp;
}

// init simulation parameters 
void scatter::simulation::load_var(const rapidjson::Document& doc){
	using namespace scatter::simulation;
	using json::load_param;
	load_param(doc, "simulation")
		("Ntraj", Ntraj)
		("mass", mass)
		("EndT", EndT)
		("Nstep", Nstep)
		("Anastep", Anastep)
		("prepinit", prepinit)
		("inittemp", inittemp)
		("initravg", initravg)
		("initpavg", initpavg)
		("vibstate", vibstate)
		("elestate", elestate)
		("initmode", initmode, enumspace::initmode_dict)
		;
	const_cast<double&>(dt) = EndT / Nstep;
	// init omega
	extract_omega();
}

// print out simulation parameters
void scatter::simulation::print_var(void){
	using namespace scatter::simulation;
	ioer::info("simulation info");
	ioer::drawline('-');
	ioer::keyval()
		("Ntraj", Ntraj)
		("mass", mass)
		("omega", omega)
		("EndT", EndT)
		("Nstep", Nstep)
		("dt", dt)
		("Anastep", Anastep)
		("prepinit", prepinit)
		("inittemp", inittemp)
		("initravg", initravg)
		("initpavg", initpavg)
		("vibstate", vibstate)
		("elestate", elestate)
		("initmode", enumspace::mode_to_string(initmode, enumspace::initmode_dict))
		;
	ioer::drawline('-');
}
