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

STRING_T scatter::io::arg_outfile;
STRING_T scatter::io::arg_logfile;
STRING_T scatter::io::arg_outdir;

STRING_T scatter::io::indir;
STRING_T scatter::io::outdir;

ioer::output_t scatter::io::out_handler;
ioer::output_t scatter::io::log_handler;

// init io parameters 
VOID_T scatter::io::load_var(VOID_T)
{
	namespace fs = boost::filesystem;
	using namespace scatter::io;

	fs::path infile(rem::infile);
	// infile & jsonfile
	infile = fs::absolute(infile);
	const STRING_T fname = infile.filename().string();
	const STRING_T stem = infile.stem().string();
	indir = infile.parent_path().string();
	if(indir[indir.length() - 1] != '/') {
		indir += "/";
	}
	jsonfile = fname + ".json";
	outdir = indir;

	// override outdir 
	if (arg_outdir.size() > 0) {
		outdir = arg_outdir;
		if(outdir[outdir.length() - 1] != '/') {
			outdir += "/";
		}
	}

	// output & log & datafile
	datfile = "." + rem::jobname + ".dat";
	initfile = "." + rem::jobname +  ".init";
	outfile = rem::jobname + ".out";
	logfile = rem::jobname + ".log";

	// overriden by command line args
	if (arg_outfile.size() > 0) {
		if (arg_outfile == "STDOUT") 
			outfile = STRING_T("");
		else
			outfile = arg_outfile;
	}

	if (arg_logfile.size() > 0) {
		if (arg_logfile == "STDOUT") 
			logfile = STRING_T("");
		else
			logfile = arg_logfile;
	}
}

// print out io parameters
VOID_T scatter::io::print_var(VOID_T)
{
	using namespace scatter::io;
	out_handler.info("io info");
	out_handler.drawline('-');
	out_handler.keyval()
		("indir", indir)
		("outdir", outdir)
		("jsonfile", jsonfile)
		("outfile", outfile)
		("logfile", logfile)
		("datfile", datfile)
		("initfile", initfile)
		;
	out_handler.drawline('-');
}

// init out & log file output manager
VOID_T scatter::io::handler_init(VOID_T)
{
	if (outfile != "") {
		out_handler.open(outdir + outfile, std::ios::out);
	}
	if (logfile != "") {
		log_handler.open(outdir + logfile, std::ios::out);
	}
}
