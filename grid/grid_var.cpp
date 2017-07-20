#include "types.hpp"
#include <vector>
#include "json_toolkit.hpp"
#include "io.hpp"
#include "grid_var.hpp"

using scatter::io::out_handler;

std::vector<DOUBLE_T> scatter::grid::rmin;
std::vector<DOUBLE_T> scatter::grid::rmax;
std::vector<UINT_T> scatter::grid::Nr;

// fef related
DOUBLE_T scatter::grid::bandwidth;
std::vector<DOUBLE_T> scatter::grid::derange;
DOUBLE_T scatter::grid::gamma_cutoff;

// init grid parameters 
VOID_T scatter::grid::load_var(const rapidjson::Document& doc){
	using namespace scatter::grid;
	using json::load_param;
	load_param(doc, "grid")
		("rmin", rmin)
		("rmax", rmax)
		("Nr", Nr)
		("bandwidth", bandwidth)
		("derange", derange)
		("gamma_cutoff", gamma_cutoff)
		;
	std::vector<DOUBLE_T> _omega;
}

// prINT_T out grid parameters
VOID_T scatter::grid::print_var(VOID_T){
	using namespace scatter::grid;
	out_handler.info("grid info");
	out_handler.drawline('-');
	out_handler.keyval()
		("rmin", rmin)
		("rmax", rmax)
		("Nr", Nr)
		("bandwidth", bandwidth)
		("derange", derange)
		("gamma_cutoff", gamma_cutoff)
		;

	out_handler.drawline('-');
}

