#include "scatter_basic.hpp"
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

VOID_T scatter::infile_parser(VOID_T) 
{
	// load path of infile & init rapidjson obj first
	io::load_var();
	rapidjson::Document&& doc = json::load_json_file(io::indir + io::jsonfile);
	rem::load_var(doc);
	// this time load datfile, initfile, etc
	io::load_var();
	grid::load_var(doc);
	surfaces::load_var(doc);
	simulation::load_var(doc);
}

int scatter::setup(int argc, char** argv) 
{
	// No argument list
	if(argc == 1) {
		cout << "No input, use -h to see help info" << "\n";
		return -1;
	}
	if(arg_parser(argc, argv) == false) {
		return -1;
	}
	// parse infile 
	try {
		infile_parser();
	} catch (const FileNotFoundError& e) {
		log_handler.info("Cannot find input file: ", io::indir + rem::infile);
		return -1;
	}
	// parse arg again to override infile
	arg_parser(argc, argv);

	// assign output/log destination
	io::handler_init();

	return 0;
}
