#include "types.hpp"
#include <string>
#include "json_toolkit.hpp"
#include "io.hpp"
#include "rem_var.hpp"
// variables in scatter::rem

using scatter::io::out_handler;

// args passed by command line
size_t scatter::rem::threadNum;
std::string scatter::rem::infile;

// job control parameters
std::string scatter::rem::jobname;
std::string scatter::rem::jobtype;
bool scatter::rem::loaddat;

// basic parameters for the system(useful both for structure & dynamics)
size_t scatter::rem::dim;
size_t scatter::rem::dim2;
double scatter::rem::hbar;
double scatter::rem::kT;
double scatter::rem::Gamma0;


// load rem parameters from Document Obj
void scatter::rem::load_var(const rapidjson::Document& doc){
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
	dim2 = dim * dim;
}

// print out rem parameters
void scatter::rem::print_var(void){
	using namespace scatter::rem;
	out_handler.info("rem info");
	out_handler.drawline('-');
	out_handler.keyval()
		("jobname", jobname)
		("jobtype", jobtype)
		("loaddat", loaddat)
		("dim", dim)
		("hbar", hbar)
		("kT", kT)
		("Gamma0", Gamma0)
		;
	out_handler.drawline('-');
}
