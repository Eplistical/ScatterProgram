#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <iostream>
#include "ioer.hpp"
#include "timer.hpp"
#include "run.hpp"
#include "io.hpp"
#include "simulation.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"

using namespace std;
using namespace scatter;

void infile_parser(void){
#if _DEBUG >= 1
	cout << "debug: infile_parser begin" << "\n";
#endif
	io::load_var();
	io::print_var();

	rapidjson::Document&& doc = json::load_json_file(io::jsonfile);

	rem::load_var(doc);
	rem::print_var();

	grid::load_var(doc);
	grid::print_var();

	surfaces::load_var(doc);
	surfaces::print_var();

	simulation::load_var(doc);
	simulation::print_var();
}

void print_vars(void){
}

int main(int argc, char** argv){
	ioer::info("Program: scatter-prepfef");
	// No argument list
	if(argc == 1){
		ioer::info("No input, use -h to see help info");
		return 0;
	}
	// program begin
	timer::now();
	timer::tic();
#if _DEBUG >= 1
	cout << "debug: debug level " << _DEBUG << "\n\n";
#endif
	// parse args
	if(arg_parser(argc, argv) == false){
		return 0;
	}
	ioer::keyval()
		("infile", rem::infile) 
		("threadNum", rem::threadNum)
		;

#if _DEBUG >= 1
	cout << "debug: parsing infile ..." << "\n\n";
#endif
	// parse infile 
	infile_parser();

	// run program
#if _DEBUG >= 1
	cout << "debug: move to the switch " << "\n";
#endif
	switch(enumspace::runmode_dict.left.at(rem::jobtype))
	{
		case enumspace::runmode_enum::SIMULATION: 
			if(simulation::prepinit){
#if _DEBUG >= 1
	cout << "debug: preparing init state " << "\n";
#endif
				simulation::generate_initstate();
				io::saveinit();
			}
#if _DEBUG >= 1
	cout << "debug: switch to run_simulation" << "\n";
#endif
			run_simulation();
			break;
		case enumspace::runmode_enum::SURFACE: 
#if _DEBUG >= 1
	cout << "debug: switch to run_surface " << "\n";
#endif
			run_surface();
			break;
		case enumspace::runmode_enum::PREPAREINIT: 
#if _DEBUG >= 1
	cout << "debug: preparing init state " << "\n";
#endif
			simulation::generate_initstate();
			io::saveinit();
			break;
		case enumspace::runmode_enum::TEST: 
			break;
	}
	// ending
	timer::toc();
	timer::now();
}
