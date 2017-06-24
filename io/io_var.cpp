#include <string>
#include "filemgr/filemgr.hpp"
#include "boost/filesystem.hpp"
#include "vars.hpp"
#include "ioer.hpp"
#include "io_var.hpp"

const std::string& scatter::io::jsonfile = "INVALID";
const std::string& scatter::io::datfile = "INVALID";
const std::string& scatter::io::initfile = "INVALID";
const std::string& scatter::io::outfile = "INVALID";

iomgr::filemgr_t scatter::io::filemgr;

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

	const_cast<std::string&>(jsonfile) = parent + fname + ".json";
	const_cast<std::string&>(datfile) = parent + "." + rem::jobname + ".dat";
	const_cast<std::string&>(initfile) = parent + "." + rem::jobname +  ".init";
	const_cast<std::string&>(outfile) = parent + rem::jobname + ".out";
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
