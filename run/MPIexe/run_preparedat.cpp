#include "types.hpp"
#include <algorithm>
#include <iterator>
#include <cassert>
#include "timer.hpp"
#include "io.hpp"
#include "run.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"
#include "grid.hpp"
#include "surfaces.hpp"
#include "MPIer.hpp"
#include "MPI_helper.hpp"

using scatter::io::out_handler;
using scatter::io::log_handler;

using namespace std;
using namespace scatter;

VOID_T run_preparedat(VOID_T)
{
	/* preparedat run: prepare force & efric & fBCME 
	 * 	for given grid & surfaces.
	 * 	save data to datfile
	 *
	 */
	using namespace grid;
	using namespace surfaces;
	using scatter::io::savedat;

	// setup objs using paras 
#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: setting up grid_obj");
#endif

	grid_obj = grid_t(rmin, rmax, Nr);

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: setting up surfaces_obj");
#endif

	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: assigning jobs");
#endif

	// assign job
	std::vector<UINT_T> mybatch = MPIer::assign_job_random(grid_obj.get_Ntot());

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: allocating fef space");
#endif

	grid_obj.alloc_fef_space();
	
#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: looping");
#endif

	// setup timer
	DOUBLE_T next_report_percent = 0.1;
	if (MPIer::master) timer::tic(99);

	// -- do job! -- //
	UINT_T index;

	std::vector<DOUBLE_T> force(mybatch.size() * rem::dim);
	std::vector<DOUBLE_T> efric(mybatch.size() * rem::dim2);
	std::vector<DOUBLE_T> fBCME(mybatch.size() * rem::dim);

	for (UINT_T i = 0, N = mybatch.size(); i < N; ++i) {
		index = mybatch[i];

		// calc fef between |0> & |1>, the index^th element
		grid_obj.calc_fef(0, 1, index, 
							&force[rem::dim * i], 
							&efric[rem::dim2 * i],
							&fBCME[rem::dim * i]);

		// timer
		if (MPIer::master and ((i + 1) / static_cast<DOUBLE_T>(N)) >= next_report_percent) {
			out_handler.info(next_report_percent * 100, " \% Done, ", timer::toc(99));
			next_report_percent += 0.1;
		}
	}

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: collecting data");
#endif

	// -- collect recorded data -- //
	std::vector<UINT_T> batch_buf;
	std::vector<DOUBLE_T> force_buf, efric_buf, fBCME_buf;

	for (UINT_T r = 1; r < MPIer::size; ++r) {
		if (MPIer::rank == r) {
			MPIer::send(0, mybatch, force, efric, fBCME);
		}
		else if (MPIer::master) {
#if _DEBUG >= 2
			if (MPIer::master) log_handler.info( "debug: receiving data from thread ", r);
#endif
			MPIer::recv(r, batch_buf, force_buf, efric_buf, fBCME_buf);
			mybatch.insert(mybatch.end(), 
							std::make_move_iterator(batch_buf.begin()),
							std::make_move_iterator(batch_buf.end())
							);
			force.insert(force.end(), 
							std::make_move_iterator(force_buf.begin()),
							std::make_move_iterator(force_buf.end())
							);
			efric.insert(efric.end(), 
							std::make_move_iterator(efric_buf.begin()),
							std::make_move_iterator(efric_buf.end())
							);
			fBCME.insert(fBCME.end(), 
							std::make_move_iterator(fBCME_buf.begin()),
							std::make_move_iterator(fBCME_buf.end())
							);
		}
		MPIer::barrier();
	}

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: sorting indices");
#endif

	// -- sort data -- //
	if (MPIer::master) {
		std::vector<UINT_T> index_vec(mybatch.size());
		std::sort(index_vec.begin(), index_vec.end(), 
					[&mybatch](UINT_T i, UINT_T j) { return mybatch[i] < mybatch[j]; });
		force_buf.clear();
		efric_buf.clear();
		fBCME_buf.clear();

		for (UINT_T i : index_vec) {
			force_buf.insert(force_buf.end(), force.begin() + i * rem::dim, force.begin() + (i + 1) * rem::dim);
			efric_buf.insert(efric_buf.end(), efric.begin() + i * rem::dim2, efric.begin() + (i + 1) * rem::dim2);
			fBCME_buf.insert(fBCME_buf.end(), fBCME.begin() + i * rem::dim, fBCME.begin() + (i + 1) * rem::dim);
		}
	}

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: combining into fef");
#endif

	// -- combine force, efric, fBCME into grid_obj.fef -- //
	if (MPIer::master) {
		std::vector<DOUBLE_T>& fef = grid_obj.get_fef_ref();
		fef.clear();
		fef.insert(fef.end(), 
					std::make_move_iterator(force_buf.begin()),
					std::make_move_iterator(force_buf.end())
					);
		fef.insert(fef.end(), 
					std::make_move_iterator(efric_buf.begin()),
					std::make_move_iterator(efric_buf.end())
					);
		fef.insert(fef.end(), 
					std::make_move_iterator(fBCME_buf.begin()),
					std::make_move_iterator(fBCME_buf.end())
					);
		log_handler.info( "fef len: ", fef.size());
	}

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: saving results");
#endif

	// savedat
	if (MPIer::master) 
		savedat();
}

int main(int argc, char** argv)
{
	// MPI setup
	MPIer::setup();

	// parse infile 
	if(MPIer::master) {
		if(setup(argc, argv) != 0) return 0; 
		assert(rem::jobtype == "preparedat");
	}

	// -- program begin -- //
	if (MPIer::master) {
		// parameter output
		out_handler.info("Program: scatter-preparedat");
		out_handler.info(timer::now());
		timer::tic();
#if _DEBUG >= 1
		if (MPIer::master) log_handler.info( "debug: debug level ", _DEBUG, "\n");
#endif
		out_handler.keyval()
			("infile", rem::infile) 
			("MPI-threadNum", MPIer::size)
			;
		rem::print_var();
		io::print_var();
		grid::print_var();
		surfaces::print_var();
	}

	// bcast vars
	scatter::bcast_vars();
	
	// run program
#if _DEBUG >= 1
	if (MPIer::master) log_handler.info( "debug: start running core part");
#endif

	run_preparedat();

#if _DEBUG >= 1
	if (MPIer::master) log_handler.info( "debug: ending");
#endif

	// ending
	if(MPIer::master) {
		out_handler.info(timer::toc());
		out_handler.info(timer::now());
	}

	MPIer::finalize();
}
