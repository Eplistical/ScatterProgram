#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "ioer.hpp"
#include "timer.hpp"
#include "io.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"
#include "mpier.hpp"

using namespace std;
using namespace scatter;

void infile_parser(void){
#if _DEBUG >= 1
	cout << "debug: print_vars begin" << "\n";
#endif
	io::load_var();
	io::print_var();

	rapidjson::Document&& doc = json::load_json_file(io::jsonfile);

	rem::load_var(doc);
	rem::jobtype = string("prepfef");
	rem::print_var();

	grid::load_var(doc);
	grid::print_var();

	surfaces::load_var(doc);
	surfaces::print_var();
}

int main(int argc, char** argv){
	// MPI setup
	mpier::setup();
	if(mpier::master) {
		ioer::info("Program: scatter-prepfef");
	}
	if(argc == 1){
		if(mpier::master) {
			ioer::info("No input, use -h to see help info");
		}
		return 0;
	}
	// program begin
	if(mpier::master) {
		timer::now();
		timer::tic();
	}
#if _DEBUG >= 1
	if(mpier::master) {
		cout << "debug: debug level " << _DEBUG << "\n\n";
	}
#endif
	// parse args
	if(arg_parser(argc, argv) == false){
		return 0;
	}
	if(mpier::master) {
		ioer::keyval()
			("infile", rem::infile) 
			("MPI-threadNum", mpier::size)
			;
	}
#if _DEBUG >= 1
	if(mpier::master) {
		cout << "debug: parsing infile ..." << "\n\n";
	}
#endif
	// parse infile 
	if(mpier::master) {
		infile_parser();
	}
	// run program
	mpier::bcast(0, rem::kT, rem::jobname, grid::rmin);
	cout << rem::kT << endl;
	cout << rem::jobname << endl;
	cout << grid::rmin[0] << endl;
	
	// ending
	if(mpier::master) {
		timer::toc();
		timer::now();
	}

	mpier::finalize();
}
