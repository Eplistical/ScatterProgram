#include <vector>
#include "json_toolkit.hpp"
#include "ioer.hpp"
#include "grid_var.hpp"

const std::vector<double>& scatter::grid::rmin = std::vector<double>();
const std::vector<double>& scatter::grid::rmax = std::vector<double>();
const std::vector<size_t>& scatter::grid::Nr  = std::vector<size_t>();

// fef related
const double& scatter::grid::bandwidth = 1.5;
const std::vector<double>& scatter::grid::derange = std::vector<double>();

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
	ioer::info("grid info");
	ioer::drawline('-');
	ioer::keyval()
		("rmin", rmin)
		("rmax", rmax)
		("Nr", Nr)
		("bandwidth", bandwidth)
		("derange", derange)
		;

	ioer::drawline('-');
}
