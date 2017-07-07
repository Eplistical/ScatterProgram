#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <iostream>
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
	// No argument list
	if(argc == 1){
		cout << "No input, use -h to see help info" << "\n";
		return 0;
	}
	if(arg_parser(argc, argv) == false){
		return 0;
	}

	// parse infile 
	infile_parser();
	// parse arg again to override infile
	arg_parser(argc, argv);
	// assign output/log destination
	io::handler_init();

	// -- program begin -- //
	out_handler.info(timer::now());
	timer::tic();
#if _DEBUG >= 1
	log_handler.info( "debug: debug level ", _DEBUG, "\n");
#endif
	out_handler.info("Program: scatter");
	out_handler.keyval()
		("infile", rem::infile) 
		("threadNum", rem::threadNum)
		;
	rem::print_var();
	io::print_var();

	// run program
#if _DEBUG >= 1
	log_handler.info( "debug: move to the switch ");
#endif
	switch(enumspace::runmode_dict.left.at(rem::jobtype))
	{
		case enumspace::runmode_enum::SIMULATION: 
			// print vars for the mode
			grid::print_var();
			surfaces::print_var();
			simulation::print_var();

			if(simulation::prepinit){
#if _DEBUG >= 1
	log_handler.info( "debug: preparing init state ");
#endif
				simulation::generate_initstate();
				io::saveinit();
			}
#if _DEBUG >= 1
	log_handler.info( "debug: switch to run_simulation");
#endif
			run_simulation();
			break;
		case enumspace::runmode_enum::SURFACE: 
			// print vars for the mode
			grid::print_var();
			surfaces::print_var();

#if _DEBUG >= 1
	log_handler.info( "debug: switch to run_surface ");
#endif
			run_surface();
			break;
		case enumspace::runmode_enum::PREPAREINIT: 
			// print vars for the mode
			grid::print_var();
			surfaces::print_var();
			simulation::print_var();

#if _DEBUG >= 1
	log_handler.info( "debug: preparing init state");
#endif
			simulation::generate_initstate();
			io::saveinit();
			break;
		case enumspace::runmode_enum::TEST: 
			break;
	}
#if _DEBUG >= 1
	log_handler.info( "debug: ending");
#endif
	// ending
	out_handler.info(timer::toc());
	out_handler.info(timer::now());
}
