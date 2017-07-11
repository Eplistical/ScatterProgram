#include "types.hpp"
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include "vars.hpp"
#include "io_var.hpp"
#include "ioer.hpp"

std::string scatter::io::jsonfile;
std::string scatter::io::datfile;
std::string scatter::io::initfile;
std::string scatter::io::outfile;
std::string scatter::io::logfile;

ioer::output_t scatter::io::out_handler;
ioer::output_t scatter::io::log_handler;

// init io parameters 
void scatter::io::load_var(void)
{
	namespace fs = boost::filesystem;
	using namespace scatter::io;

	fs::path infile(rem::infile);
	infile = fs::absolute(infile);
	std::string parent = infile.parent_path().string();
	if(parent[parent.length() - 1] != '/'){
		parent += "/";
	}
	const std::string fname = infile.filename().string();
	const std::string stem = infile.stem().string();

	jsonfile = parent + fname + ".json";
	datfile = parent + "." + rem::jobname + ".dat";
	initfile = parent + "." + rem::jobname +  ".init";
	outfile = parent + rem::jobname + ".out";
	logfile = parent + rem::jobname + ".log";
}

// print out io parameters
void scatter::io::print_var(void)
{
	using namespace scatter::io;
	out_handler.info("io info");
	out_handler.drawline('-');
	out_handler.keyval()
		("jsonfile", jsonfile)
		("datfile", datfile)
		("initfile", initfile)
		("outfile", outfile)
		;
	out_handler.drawline('-');
}

void scatter::io::handler_init(void)
{
	out_handler.open(outfile, std::ios::out);
	log_handler.open(logfile, std::ios::out);
}
