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

// init grid parameters 
void scatter::grid::load_var(const rapidjson::Document& doc){
	using namespace scatter::grid;
	using json::load_param;
	load_param(doc, "grid")
		("rmin", rmin)
		("rmax", rmax)
		("Nr", Nr)
		("bandwidth", bandwidth)
		("derange", derange)
		;
	std::vector<DOUBLE_T> _omega;
}

// prINT_T out grid parameters
void scatter::grid::print_var(void){
	using namespace scatter::grid;
	out_handler.info("grid info");
	out_handler.drawline('-');
	out_handler.keyval()
		("rmin", rmin)
		("rmax", rmax)
		("Nr", Nr)
		("bandwidth", bandwidth)
		("derange", derange)
		;

	out_handler.drawline('-');
}

