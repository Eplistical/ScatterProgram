#include <iostream>
#include "ioer.hpp"
#include "timer.hpp"
#include "run.hpp"
#include "vars.hpp"
#include "io.hpp"
using namespace std;
using namespace scatter;

void print_vars(void){
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
	timer::now();
	timer::tic();
	// parse args
	if(arg_parser(argc, argv) == false){
		return 0;
	}
	// parse infile 
	infile_parser();
	print_vars();
	// run program
	switch(enumspace::runmode_dict.left.at(rem::jobtype))
	{
		case enumspace::runmode_enum::SIMULATION: 
			//run_simulation();
			break;
		case enumspace::runmode_enum::SURFACE: 
			run_surface();
			break;
		case enumspace::runmode_enum::TEST: 
			break;
	}
	// ending
	timer::toc();
	timer::now();
}
