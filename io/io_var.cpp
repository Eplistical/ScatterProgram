#include <string>
#include "boost/filesystem.hpp"
#include "vars.hpp"
#include "ioer.hpp"
#include "io_var.hpp"

std::string scatter::io::jsonfile;
std::string scatter::io::datfile;
std::string scatter::io::initfile;
std::string scatter::io::outfile;

// init io parameters 
void scatter::io::load_var(void){
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
}

// print out io parameters
void scatter::io::print_var(void){
	using namespace scatter::io;
	ioer::info("io info");
	ioer::drawline('-');
	ioer::keyval()
		("jsonfile", jsonfile)
		("datfile", datfile)
		("initfile", initfile)
		("outfile", outfile)
		;
	ioer::drawline('-');
}
