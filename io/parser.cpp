#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <iostream>
#include <string>
#include <stdexcept>
#include "vars.hpp"
#include "parser.hpp"
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace scatter;

static po::variables_map _parse_arg(int argc, char** argv){
	// setup
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "print help message")
		("infile,i", po::value<std::string>(), "input file path")
		("nproc,n", po::value<size_t>(), "# of processors" )
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
bool scatter::arg_parser(int argc, char** argv){
	po::variables_map&& vm = _parse_arg(argc, argv);
	if(vm.count("help")){
		return false;
	}
	if(vm.count("infile")){
		rem::infile = vm["infile"].as<std::string>();
		// check if infile exists
		fs::path tmp(rem::infile);
		if(not fs::exists(tmp)){
			throw std::invalid_argument("input file not exists!");
		}
	}
	if(vm.count("nproc")){
		rem::threadNum = vm["nproc"].as<size_t>();
	}
	return true;
}
