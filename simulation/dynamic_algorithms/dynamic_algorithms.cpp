#include "scatter_basic.hpp"
#include "dynamic_algorithms.hpp"

using namespace scatter::simulation;

std::unordered_map<enumspace::dynamic_mode_enum, DynamicFunctionPtrType>
scatter::simulation::dynamic_algorithms
{
	{ enumspace::dynamic_mode_enum::CME, &CME },
	{ enumspace::dynamic_mode_enum::BCME, &BCME },
	{ enumspace::dynamic_mode_enum::EF, &EF },
};
