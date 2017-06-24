#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <iostream>
#include "ioer.hpp"
#include "timer.hpp"
#include "run.hpp"
#include "vars.hpp"
#include "io.hpp"
#include <Eigen/Core>
using namespace std;
using namespace scatter;

void print_vars(void){
#ifdef _DEBUG
	cout << "print_vars begin" << "\n";
#endif
	ioer::keyval()
		("threadNum", rem::threadNum)
		("infile", rem::infile) 
		;
	rem::print_var();
	io::print_var();
	grid::print_var();
	surfaces::print_var();
	simulation::print_var();
}

int main(int argc, char** argv){
	// No argument list
	if(argc == 1){
		std::cout 
			<< "No input, use -h to see help info"
			<< std::endl;
		return 0;
	}
	// program begin
	timer::now();
	timer::tic();
#ifdef _DEBUG
	cout << "main: debug version" << "\n\n";
#endif
	// parse args
	if(arg_parser(argc, argv) == false){
		return 0;
	}
#ifdef _DEBUG
	cout << "parsing infile ..." << "\n\n";
#endif
	// parse infile 
	infile_parser();
	print_vars();
	// run program
	switch(enumspace::runmode_dict.left.at(rem::jobtype))
	{
		case enumspace::runmode_enum::SIMULATION: 
			if(simulation::prepinit){
				simulation::generate_initstate();
				io::saveinit();
			}
			run_simulation();
			break;
		case enumspace::runmode_enum::SURFACE: 
			run_surface();
			break;
		case enumspace::runmode_enum::PREPAREINIT: 
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
