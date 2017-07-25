#include "scatter_basic.hpp"
#include <iostream>
#include <stdexcept>
#include "vars.hpp"
#include "parser.hpp"
#include "scatter_exceptions.hpp"
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace scatter;

static po::variables_map _parse_arg(INT_T argc, char** argv){
	// setup
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "print help message")
		("infile,i", po::value<STRING_T>(), "input file path")
		("nproc,n", po::value<UINT_T>(), "# of processors" )
		("outfile,o", po::value<STRING_T>(), "output file path")
		("logfile,l", po::value<STRING_T>(), "log file path")
		("outdir,s", po::value<STRING_T>(), "directory for output")
		;
	// parse and store rst to vm, may throw
	po::variables_map vm; 
	po::store(po::parse_command_line(argc, argv, desc), vm); 
	// if help msg
	if(vm.count("help")){
		std::cout 
			<< "Basic usage: " << std::endl
			<< desc << std::endl;
	}

	return vm;
}

// API
BOOL_T scatter::arg_parser(INT_T argc, char** argv){
	po::variables_map&& vm = _parse_arg(argc, argv);
	if(vm.count("help")){
		return false;
	}
	if(vm.count("infile")){
		rem::infile = vm["infile"].as<STRING_T>();
		// check if infile exists
		fs::path tmp(rem::infile);
		if(not fs::exists(tmp)){
			throw scatter::FileNotFoundError("arg parser: input file not exists!");
		}
	}
	if(vm.count("nproc")){
		rem::threadNum = vm["nproc"].as<UINT_T>();
	}
	if(vm.count("outfile")){
		io::arg_outfile = vm["outfile"].as<STRING_T>();
	}
	if(vm.count("logfile")){
		io::arg_logfile = vm["logfile"].as<STRING_T>();
	}
	if(vm.count("outdir")){
		io::arg_outdir = vm["outdir"].as<STRING_T>();
	}
	return true;
}
