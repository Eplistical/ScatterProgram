#include "scatter_basic.hpp"
#include "json_toolkit.hpp"
#include "io.hpp"
#include "rem_var.hpp"
// variables in scatter::rem
using scatter::io::out_handler;

// args passed by command line
UINT_T scatter::rem::threadNum = 1;
STRING_T scatter::rem::infile;

// job control parameters
STRING_T scatter::rem::jobname;

// basic parameters for the system(useful both for structure & dynamic)
UINT_T scatter::rem::dim;
UINT_T scatter::rem::dim2;
DOUBLE_T scatter::rem::hbar;
DOUBLE_T scatter::rem::kT;
DOUBLE_T scatter::rem::Gamma0;


// load rem parameters from Document Obj
VOID_T scatter::rem::load_var(const rapidjson::Document& doc)
{
	using namespace scatter::rem;
	using json::load_param;
	load_param(doc, "rem")
		("jobname", jobname)
		("dim", dim)
		("hbar", hbar)
		("kT", kT)
		("Gamma0", Gamma0)
		;
	dim2 = dim * dim;
}

// prINT_T out rem parameters
VOID_T scatter::rem::print_var(VOID_T)
{
	using namespace scatter::rem;
	out_handler.info("rem info");
	out_handler.drawline('-');
	out_handler.keyval()
		("jobname", jobname)
		("dim", dim)
		("hbar", hbar)
		("kT", kT)
		("Gamma0", Gamma0)
		;
	out_handler.drawline('-');
}
