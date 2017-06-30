#include <vector>
#include "ioer.hpp"
#include "json_toolkit.hpp"
#include "surfaces_collection.hpp"
#include "string_mode_conversion.hpp"
#include "surfaces_var.hpp"

namespace NS = scatter::surfaces;
using enumspace::surfmode_enum;

const double& NS::cutoff_gamma = 0.0;

const size_t& NS::surfnum = 0;
const std::vector<surfmode_enum>& NS::surfmode = std::vector<surfmode_enum>();
const std::vector<std::vector<double>>& NS::surfpara = std::vector<std::vector<double>>();
const std::vector<surfmode_enum>& NS::gammamode = std::vector<surfmode_enum>();
const std::vector<std::vector<double>>& NS::gammapara = std::vector<std::vector<double>>(); 

// init surfaces parameters 
void scatter::surfaces::load_var(const rapidjson::Document& doc){
	using namespace scatter::surfaces;
	using json::load_param;
	load_param(doc, "surfaces")
		("surfnum", surfnum)
		("cutoff_gamma", cutoff_gamma)
		("surfmode", surfmode, enumspace::surfmode_dict)
		("surfpara", surfpara)
		("gammamode", gammamode, enumspace::surfmode_dict)
		("gammapara", gammapara)
		;
}

// print out surfaces parameters
void scatter::surfaces::print_var(void){
	using namespace scatter::surfaces;
	ioer::info("surfaces info");
	ioer::drawline('-');
	ioer::keyval()
		("surfnum", surfnum)
		("cutoff_gamma", cutoff_gamma)
		;

	ioer::keyval()("surfmode", "");
	for(auto& it : surfmode){
		ioer::keyval(std::cout, "   ")("", enumspace::mode_to_string(it, enumspace::surfmode_dict));
	}

	ioer::keyval()("surfpara", "");
	for(auto& it : surfpara){
		ioer::keyval(std::cout, "   ")("", it);
	}

	ioer::keyval()("gammamode", "");
	for(auto& it : gammamode){
		ioer::keyval(std::cout, "   ")("", enumspace::mode_to_string(it, enumspace::surfmode_dict));
	}

	ioer::keyval()("gammapara", "");
	for(auto& it : gammapara){
		ioer::keyval(std::cout, "   ")("", it);
	}

	ioer::drawline('-');
}
