#include "types.hpp"
#include <iostream>
#include "boost/filesystem.hpp"
#include "vars.hpp"
#include "io_var.hpp"
#include "ioer.hpp"

STRING_T scatter::io::jsonfile;
STRING_T scatter::io::datfile;
STRING_T scatter::io::initfile;
STRING_T scatter::io::outfile;
STRING_T scatter::io::logfile;

STRING_T scatter::io::parent_dir;

ioer::output_t scatter::io::out_handler;
ioer::output_t scatter::io::log_handler;

// init io parameters 
void scatter::io::load_var(void)
{
	namespace fs = boost::filesystem;
	using namespace scatter::io;

	fs::path infile(rem::infile);
	infile = fs::absolute(infile);
	parent_dir = infile.parent_path().string();
	if(parent_dir[parent_dir.length() - 1] != '/'){
		parent_dir += "/";
	}

	const STRING_T fname = infile.filename().string();
	const STRING_T stem = infile.stem().string();

	jsonfile = parent_dir + fname + ".json";
	datfile = parent_dir + "." + rem::jobname + ".dat";
	initfile = parent_dir + "." + rem::jobname +  ".init";
	outfile = parent_dir + rem::jobname + ".out";
	logfile = parent_dir + rem::jobname + ".log";
}

// prINT_T out io parameters
void scatter::io::print_var(void)
{
	using namespace scatter::io;
	out_handler.info("io info");
	out_handler.drawline('-');
	out_handler.keyval()
		("parent_dir", parent_dir)
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
