#include "types.hpp"
#include <vector>
#include "io.hpp"
#include "json_toolkit.hpp"
#include "surfaces_collection.hpp"
#include "string_mode_conversion.hpp"
#include "surfaces_var.hpp"

using scatter::io::out_handler;

namespace NS = scatter::surfaces;
using enumspace::surfmode_enum;

DOUBLE_T NS::cutoff_gamma;

UINT_T NS::surfnum;
std::vector<surfmode_enum> NS::surfmode;
std::vector<std::vector<DOUBLE_T> > NS::surfpara;
std::vector<surfmode_enum> NS::gammamode;
std::vector<std::vector<DOUBLE_T> > NS::gammapara; 

// init surfaces parameters 
VOID_T scatter::surfaces::load_var(const rapidjson::Document& doc)
{
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

// prINT_T out surfaces parameters
VOID_T scatter::surfaces::print_var(VOID_T)
{
	using namespace scatter::surfaces;
	out_handler.info("surfaces info");
	out_handler.drawline('-');
	out_handler.keyval()
		("surfnum", surfnum)
		("cutoff_gamma", cutoff_gamma)
		;

	out_handler.keyval()("surfmode", "");
	for(auto& it : surfmode) {
		out_handler.keyval()("", enumspace::mode_to_string(it, enumspace::surfmode_dict));
	}

	out_handler.keyval()("surfpara", "");
	for(auto& it : surfpara) {
		out_handler.keyval()("", it);
	}

	out_handler.keyval()("gammamode", "");
	for(auto& it : gammamode) {
		out_handler.keyval()("", enumspace::mode_to_string(it, enumspace::surfmode_dict));
	}

	out_handler.keyval()("gammapara", "");
	for(auto& it : gammapara) {
		out_handler.keyval()("", it);
	}

	out_handler.drawline('-');
}
