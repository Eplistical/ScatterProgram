#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <boost/version.hpp>
#include "rem/rem.hpp"
#include "io/io.hpp"
#include "arg_parser.hpp"
namespace po = boost::program_options;
using namespace scatter;

static po::variables_map _parse(int argc, char** argv) {
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
bool scatter::arg_parser(int argc, char** argv) noexcept{
	po::variables_map&& vm = _parse(argc, argv);
	if(vm.count("help")){
		return false;
	}
	else{
		const_cast<std::string&>(io::infile) = vm["infile"].as<std::string>();
		const_cast<size_t&>(rem::threadNum) = vm["threadNum"].as<size_t>();
	}
}
