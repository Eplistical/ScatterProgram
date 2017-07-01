#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include "ioer_exceptions.hpp"
#include "ioer_base_t.hpp"
using namespace ioer;

// helpers
bool ioer_base_t::path_exist(const std::string& path) const noexcept{
	return (_fpdict.find(path) != _fpdict.end());
}

bool ioer_base_t::is_stdio(const std::string& path) const noexcept{
	return(path == ioer::stdio_path);
}

// desctructor
ioer_base_t::~ioer_base_t(){
	for(auto it = _fpdict.begin(); it != _fpdict.end(); ++it){
		close(it->first);
	}
}

// open/close a source 
void ioer_base_t::open(const std::string& path, std::ios::openmode mode){
	if(path != stdio_path){
		if(path_exist(path)) return;
		_fpdict.insert(std::pair<std::string, std::fstream>
				(path, std::fstream(path, mode))
				);
		if(not _fpdict.at(path).is_open()){
			throw FileNotOpenedError("Unable to open file " + path);
		}
	}
}

void ioer_base_t::close(const std::string& path){
	if(path != stdio_path){
		try{
			_fpdict.at(path).close();
			_fpdict.erase(path);
		}catch(const std::out_of_range& e){
			throw FileNotOpenedError
				("Can't close file " + path + ", it is not opened");
		}
	}
}

// getter
size_t ioer_base_t::size(void) const noexcept{
	return _fpdict.size(); 
}

// access to iostream object
std::iostream& ioer_base_t::at(const std::string& path){
	if(is_stdio(path)){
		return static_cast<std::iostream&>(std::cout);
	}
	else{
		try{
			return static_cast<std::iostream&>(_fpdict.at(path));
		}catch(const std::out_of_range& e){
			throw FileNotOpenedError
				("at: file not opened");
		}
	}
}

