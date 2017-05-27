// basic
#include <omp.h>
#include "aux/timer.hpp"
// scatter related
#include "scatter.hpp"
#include "arg_parser.hpp"
#include "runmode.hpp"
#include <iostream>
using namespace std;
using namespace scatter;

int main(int argc, char** argv){
	timer::now();
	timer::tic();
	// parse args
	arg_parser(argc, argv);
	// environment
	omp_set_num_threads(rem::threadNum);
	cout << rem::threadNum << endl;
	// run program
	switch(mode::run_mode_dict.left.at(rem::jobtype))
	{
		case mode::runmode::SIMULATION: 
			break;
		case mode::runmode::SURFACE: 
			break;
		case mode::runmode::TEST: 
			break;
	}
	// ending
	timer::toc();
	timer::now();
	return 0;
}
