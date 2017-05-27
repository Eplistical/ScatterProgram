// basic
#include <omp.h>
#include "aux/timer.hpp"
// scatter related
#include "scatter.hpp"
#include "arg_parser.hpp"
using namespace scatter;

int main(int argc, char** argv){
	timer::now();
	timer::tic();
	// parse arguments
	// environment
	cout.precision(8);
	mkl_set_num_threads(rem::threadNum);
	omp_set_num_threads(rem::threadNum);
	IO::keyval("threadNum", threadNum, true);
	IO::keyval("infile", infile, true);
	// run program
	switch(runmode){
		case RUNMODE::TEST :
			test(para);
			break;
		case RUNMODE::PREP : 
			para.prepr0p0();
			break;
		case RUNMODE::SIMULATION : 
			para.loadr0p0();
			runsimu(para);
			break;
		case RUNMODE::DAT : 
			loaddat = 0;
			para.prepdat();
			FileMgr::savedat(para);
			break;
		case RUNMODE::SURF : 
			shape(para, 0);
			shape(para, 1);
			xzshape(para);
			break;
		default : 
			throw std::out_of_range("Unknown mode");
			break;
	}
	// ending
	IO::info("Program Ends");
	Timer::toc();
	timer::now();
	return 0;
}
