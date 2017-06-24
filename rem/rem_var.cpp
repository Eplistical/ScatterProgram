#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <string>
#include "json_toolkit.hpp"
#include "ioer.hpp"
#include "rem_var.hpp"
// variables in scatter::rem


// args passed by command line
const size_t& scatter::rem::threadNum = 1;
const std::string& scatter::rem::infile = "test.in";

// job control parameters
const std::string& scatter::rem::jobname = "test";
const std::string& scatter::rem::jobtype = "simulation";
const bool& scatter::rem::loaddat = true;

// basic parameters for the system(useful both for structure & dynamics)
const size_t& scatter::rem::dim = 1;
const size_t& scatter::rem::dim2 = scatter::rem::dim * scatter::rem::dim;
const double& scatter::rem::hbar = 1.0;
const double& scatter::rem::kT = 0;
const double& scatter::rem::Gamma0 = 0;


// load rem parameters from Document Obj
void scatter::rem::load_var(const rapidjson::Document& doc){
#ifdef _DEBUG
	cout << "rem::load_var begin" << "\n";
#endif

	using namespace scatter::rem;
	using json::load_param;
	load_param(doc, "rem")
		("jobname", jobname)
		("jobtype", jobtype)
		("loaddat", loaddat)
		("dim", dim)
		("hbar", hbar)
		("kT", kT)
		("Gamma0", Gamma0)
		;
	const_cast<size_t&>(dim2) = dim * dim;
}

// print out rem parameters
void scatter::rem::print_var(void){
	using namespace scatter::rem;
	ioer::info("rem info");
	ioer::drawline('-');
	ioer::keyval()
		("jobname", jobname)
		("jobtype", jobtype)
		("loaddat", loaddat)
		("dim", dim)
		("hbar", hbar)
		("kT", kT)
		("Gamma0", Gamma0)
		;
	ioer::drawline('-');
}
