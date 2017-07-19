#include "scatter_basic.hpp"
#include <sstream>
#include "vector.hpp"
#include "matrixop.hpp"
#include "randomer.hpp"
#include "rem.hpp"
#include "simulation.hpp"
#include "evolve.hpp"
#include "dynamics_mode.hpp"
#include "run.hpp"

using scatter::grid_obj;
using namespace scatter;

using rem::dim;
using rem::kT;

using simulation::mass;
using simulation::dt;
using simulation::velocity_verlet;

static std::vector<DOUBLE_T> _EF_get_Force(const particle_t& ptcl, enumspace::dynamics_mode_enum mode) 
{
    // force
    std::vector<DOUBLE_T>&& force = grid_obj.get_force(ptcl.r);
    // [efric] * [p]
    std::vector<DOUBLE_T>&& efric_p = matrixop::matvec(grid_obj.get_efric(ptcl.r), ptcl.p);
    return force - efric_p / mass + ptcl.ranforce;
}

static void _EF_get_Ranforce(particle_t& ptcl, enumspace::dynamics_mode_enum mode)
{
    std::vector<DOUBLE_T>&& force = grid_obj.get_force(ptcl.r);
    std::vector<DOUBLE_T>&& efric = grid_obj.get_efric(ptcl.r);

    // if efric << force, ranforce = 0
    if(norm(efric) < 1e-8 * min(abs(force))) {
		ptcl.ranforce.assign(dim, 0);
    }

    std::vector<DOUBLE_T> eva, evt;
	matrixop::hdiag(efric, eva, evt);

    // deal with negetive eva
	DOUBLE_T meaneva = mean(eva);
	DOUBLE_T sigma;

	// negative but very close to 0 (likely to be a numerical error) set to 0
	for (auto& it : eva) {
		if (it < 0.0) {
			if(abs(it) < 1e-6 * meaneva) {
				it = 0.0;
			}
			else {
				std::ostringstream errmsg;
				std::cout << it << " <-neg " << std::endl;
				errmsg << "Negative eva found: " << it;
				throw scatter::NegativeEigenValueError(errmsg.str());
			}
		}
		// get gaussian random
		sigma = 2.0 * kT * it / dt;
		it = randomer::normal(0.0, sigma);
	}
    ptcl.ranforce =  matrixop::matvec(evt, eva);
}

static VOID_T _EF(particle_t& ptcl, enumspace::dynamics_mode_enum mode) {
    _EF_get_Ranforce(ptcl, mode);
    velocity_verlet(ptcl, mode, _EF_get_Force);
}

// API
VOID_T scatter::simulation::EF(particle_t& ptcl, UINT_T trajID) {
	_EF(ptcl, enumspace::dynamics_mode_enum::EF);
}

