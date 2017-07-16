#include "types.hpp"
#include "dynamic_algorithms.hpp"

using namespace scatter::simulation;

std::unordered_map<enumspace::dynamics_mode_enum, DynamicFunctionPtrType>
scatter::simulation::dynamic_algorithms
{
	{ enumspace::dynamics_mode_enum::CME, &CME },
	{ enumspace::dynamics_mode_enum::BCME, &BCME },
	{ enumspace::dynamics_mode_enum::EF, &EF },
};
