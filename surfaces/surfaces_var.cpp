#include "scatter_basic.hpp"
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

// print out surfaces parameters
VOID_T scatter::surfaces::print_var(VOID_T)
{
	using namespace scatter::surfaces;
	out_handler.info("surfaces info");
	out_handler.drawline('-');
	out_handler.keyval()
		("surfnum", surfnum)
		("cutoff_gamma", cutoff_gamma)
		;

	UINT_T dim = surfmode.size() / surfnum;

	out_handler.info("surfaces para: \n");
	for (UINT_T i = 0; i < surfnum; ++i) {
		for (UINT_T d = 0; d < dim; ++d) {
			out_handler.info("i = ", i, ", d = ", d, "\n\n", get_surface_expr(surfmode[d], surfpara[d]));
		}
	}

	out_handler.info("gamma para: \n");
	for (UINT_T d = 0; d < dim; ++d) {
		out_handler.info("d = ", d, "\n\n", get_surface_expr(gammamode[d], gammapara[d]));
	}

	out_handler.drawline('-');
}

STRING_T scatter::surfaces::get_surface_expr(enumspace::surfmode_enum mode, const std::vector<DOUBLE_T>& para)
{
	using namespace surfaces_collection;
	using namespace enumspace;
	switch(mode){
		case surfmode_enum::CONSTANT:
			return constant(para).get_expr(); break;
		case surfmode_enum::STEP:
			return step(para).get_expr(); break;
		case surfmode_enum::HARMONIC:
			return harmonic(para).get_expr(); break;
		case surfmode_enum::FERMI:
			return fermi(para).get_expr(); break;
		case surfmode_enum::GAUSSIAN:
			return gaussian(para).get_expr(); break;
		case surfmode_enum::MORSE:
			return morse(para).get_expr(); break;
		case surfmode_enum::NEWNS:
			return newns(para).get_expr(); break;
		default:
			throw scatter::InvalidModeError("cannot find surfmode: " + surfmode_dict.right.at(mode));
	}
}
