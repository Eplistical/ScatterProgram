#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "timer.hpp"
#include "io.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"
#include "mpier.hpp"

using scatter::io::out_handler;
using scatter::io::log_handler;

using namespace std;
using namespace scatter;

void infile_parser(void){
	// load path of infile & init rapidjson obj first
	io::load_var();
	rapidjson::Document&& doc = json::load_json_file(io::jsonfile);
	rem::load_var(doc);
	// this time load datfile, initfile, etc
	io::load_var();
	grid::load_var(doc);
	surfaces::load_var(doc);
	simulation::load_var(doc);
}

int main(int argc, char** argv){
	// MPI setup
	mpier::setup();
	if(argc == 1){
		if(mpier::master) 
			std::cout << "No input, use -h to see help info\n";
		return 0;
	}
	// parse args
	if(arg_parser(argc, argv) == false){
		return 0;
	}

	// parse infile 
	if(mpier::master) {
		// parse infile 
		infile_parser();
		// parse arg again to override infile
		arg_parser(argc, argv);
		// assign output/log destination
		io::handler_init();
	}

	// -- program begin -- //
	if (mpier::master) {
		out_handler.info("Program: scatter-prepfef");
		out_handler.info(timer::now());
		timer::tic();
#if _DEBUG >= 1
		log_handler.info( "debug: debug level ", _DEBUG, "\n");
#endif
		out_handler.keyval()
			("infile", rem::infile) 
			("MPI-threadNum", mpier::size)
			;
	}

	// run program
	mpier::bcast(0, rem::kT, rem::jobname, grid::rmin);
	if(mpier::master) {
		out_handler.info(rem::kT, rem::jobname, grid::rmin);
	}
	
	// ending
	if(mpier::master) {
		out_handler.info(timer::toc());
		out_handler.info(timer::now());
	}

	mpier::finalize();
}
