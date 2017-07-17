#include "types.hpp"
#include "io.hpp"
#include "vector.hpp"
#include "randomer.hpp"
#include "rem.hpp"
#include "simulation.hpp"
#include "initmode.hpp"
using scatter::io::out_handler;

using namespace scatter;

VOID_T simulation::generate_initstate(VOID_T){
	using namespace scatter::rem;
	using namespace scatter::simulation;
	using enumspace::initmode_enum;

	// get simulation properties
	const std::vector<DOUBLE_T> sigmar = pow(inittemp / omega.at(elestate) / omega.at(elestate) / mass, 0.5);
	const std::vector<DOUBLE_T> sigmap = pow(inittemp * mass, 0.5);
	out_handler.keyval()
		("sigmap ", sigmap)
		("initravg ", initravg)
		("initpavg ", initpavg);

	// prepare r0p0
	std::vector<DOUBLE_T> r, p, rp;
	DOUBLE_T tmp;
	// loop over Ntraj
	for(UINT_T traj = 0; traj < Ntraj; ++traj){
		r.clear();
		p.clear();
		// loop over dim
		for(UINT_T d = 0; d < dim; ++d){
			if(initmode.at(d) == initmode_enum::DELTA){
				r.push_back(initravg.at(d));
				p.push_back(initpavg.at(d));
			}
			else if(initmode.at(d) == initmode_enum::RING){
				tmp = randomer::rand(0, 2 * M_PI);
				r.push_back(sqrt(2 * vibstate + 1.0) * cos(tmp) * sqrt(1.0 / mass.at(0) / omega.at(elestate) / hbar));
				p.push_back(sqrt(2 * vibstate + 1.0) * sin(tmp) * sqrt(mass.at(0) * omega.at(elestate) * hbar));
			}
			else if(initmode.at(d) == initmode_enum::RP_GAUSSIAN){
				r.push_back(randomer::normal(initravg.at(d), sigmar.at(d)));
				p.push_back(randomer::normal(initpavg.at(d), sigmap.at(d)));
			}
			else if(initmode.at(d) == initmode_enum::P_GAUSSIAN){
				r.push_back(initravg.at(d));
				p.push_back(randomer::normal(initpavg.at(d), sigmap.at(d)));
			}
			else if(initmode.at(d) == initmode_enum::P_GAUSSIAN_NON_NEGATIVE){
				r.push_back(initravg.at(d));
				p.push_back(abs(randomer::normal(initpavg.at(d), sigmap.at(d))));
			}
		}
		// insert r, p for traj^th particle to rp
		rp.insert(rp.end(), r.begin(), r.end());
		rp.insert(rp.end(), p.begin(), p.end());
	}

	r0p0 = rp;
}
