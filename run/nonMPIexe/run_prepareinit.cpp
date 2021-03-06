#include "scatter_basic.hpp"
#include <iostream>
#include <cassert>
#include "timer.hpp"
#include "io.hpp"
#include "run.hpp"
#include "simulation.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"

using scatter::io::out_handler;
using scatter::io::log_handler;

using namespace std;
using namespace scatter;

using namespace scatter;
using namespace scatter::grid;
using namespace scatter::surfaces;
using namespace scatter::simulation;
using namespace scatter::rem;

VOID_T assign_initstate(std::vector<particle_t>& swarm)
{
	for(UINT_T traj = 0; traj < Ntraj; ++traj){
		swarm.at(traj).ranforce = std::vector<DOUBLE_T>(dim, 0.0);
		swarm.at(traj).surf = 0;
		swarm.at(traj).r = std::vector<DOUBLE_T>(r0p0.begin() + traj * dim * 2,
												r0p0.begin() + traj * dim * 2 + dim);
		swarm.at(traj).p = std::vector<DOUBLE_T>(r0p0.begin() + traj * dim * 2 + dim,
												r0p0.begin() + traj * dim * 2 + dim * 2);
	}
}

VOID_T run_prepareinit()
{
#if _DEBUG >= 1
	log_handler.info( "debug: run_prepareinit: begin");
#endif
	simulation::generate_initstate();
	io::saveinit();
}

int main(int argc, char** argv){
	if(setup(argc, argv) != 0) return 0; 
	// -- program begin -- //
	out_handler.info(timer::now());
	timer::tic();
#if _DEBUG >= 1
	log_handler.info( "debug: debug level ", _DEBUG, "\n");
#endif
	// parameter output
	out_handler.info("Program: scatter-prepareinit");
	out_handler.keyval()
		("infile", rem::infile) 
		("threadNum", rem::threadNum)
		;
	rem::print_var();
	io::print_var();
	// run program
#if _DEBUG >= 1
	log_handler.info( "debug: start running core part");
#endif
	run_prepareinit();
#if _DEBUG >= 1
	log_handler.info( "debug: ending");
#endif
	// ending
	out_handler.info(timer::toc());
	out_handler.info(timer::now());
}
