#include <vector>
#include "json_toolkit.hpp"
#include "io.hpp"
#include "grid_var.hpp"

using scatter::io::out_handler;

std::vector<double> scatter::grid::rmin;
std::vector<double> scatter::grid::rmax;
std::vector<size_t> scatter::grid::Nr;

// fef related
double scatter::grid::bandwidth;
std::vector<double> scatter::grid::derange;

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
	std::vector<double> _omega;
}

// print out grid parameters
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

