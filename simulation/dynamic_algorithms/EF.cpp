#include "types.hpp"
#include "vector.hpp"
#include "matrixop.hpp"
#include "randomer.hpp"
#include "rem.hpp"
#include "simulation.hpp"
#include "evolve.hpp"
#include "dynamics_mode.hpp"

using namespace scatter;
using namespace scatter::simulation;
const INT_T dim = scatter::rem::dim;

static std::vector<DOUBLE_T> _EF_get_Force(particle_t& ptcl, enumspace::dynamics_mode_enum mode) {
    // m enters with ranforce
    const std::vector<DOUBLE_T> force;// = para.get_force(ptcl.r);
    const std::vector<DOUBLE_T> efric;// = para.get_efric(ptcl.r);
    // efric_p = [efric] * [p]
    std::vector<DOUBLE_T>&& efric_p = matrixop::matvec(efric, ptcl.p);
    return force - efric_p / mass + ptcl.ranforce;
}

static std::vector<DOUBLE_T> _EF_get_Ranforce(particle_t& ptcl, enumspace::dynamics_mode_enum mode){
    std::vector<DOUBLE_T> ranforce;
	/*
    const std::vector<DOUBLE_T> efric = para.get_efric(ptcl.r);
    // if efric << force, ranforce = 0
    const std::vector<DOUBLE_T> force = para.get_force(ptcl.r);
    if(norm(efric) < 1e-8 * min(abs(force))){
        ranforce.assign(dim, 0.0);
        return ranforce;
    }
    std::vector<DOUBLE_T> eva, evt;
	matrixop::hdiag(efric, eva, evt);

    // deal with negetive eva
    if(eva[0] < 0.0 or eva[1] < 0.0){
        // one eva is negative but very close to 0 
		// (likely to be a numerical error)
		// set it to 0
        if(eva[0] < 0.0 and abs(eva[0]) < 1e-6 * abs(eva[1])){
            eva[0] = 0.0;
        }
        else if(eva[1] < 0.0 and abs(eva[1]) < 1e-6 * abs(eva[0])){
            eva[1] = 0.0;
        }
    }
    // gaussian random
    for(INT_T d = 0; d < dim; ++d){
		eva[0] = randomer::normal(0.0, sqrt(2.0 * rem::kT * eva[d] / dt));
    }
    ranforce =  matrixop::matvec(evt, eva);
	*/
    return ranforce;
}

static VOID_T _EF(particle_t& ptcl, enumspace::dynamics_mode_enum mode) {
    ptcl.ranforce = _EF_get_Ranforce(ptcl, mode);
    //velocity_verlet(ptcl, mode, _EF_get_Force);
}

// API
VOID_T scatter::simulation::EF(particle_t& ptcl, UINT_T trajID) {
	_EF(ptcl, enumspace::dynamics_mode_enum::EF);
}

