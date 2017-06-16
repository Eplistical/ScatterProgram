#include <cassert>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "iomgr.hpp"
using namespace io;

// desctructor
iomgr_t::~iomgr_t(){
	for(auto it = _fpdict.begin(); it != _fpdict.end(); ++it){
		close(it->first);
	}
}

// getter
size_t iomgr_t::size(void){ 
	return _fpdict.size(); 
}

// open/close a source 
void iomgr_t::open(const std::string& path, std::ios::openmode mode){
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

void iomgr_t::close(const std::string& path){
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
// read/write binary data
void iomgr_t::read(const std::string& path, char* offset, size_t len){
	if(path != stdio_path){
		try{
			_fpdict.at(path).read(offset, len);
		}catch(const std::out_of_range& e){
			throw FileNotOpenedError
				("Can't read from file " + path + ", it is not opened");
		}
	}
	else{
		throw FileModeIncompatibleError
			("Can't read binary data from stdio_path");
	}
}

void iomgr_t::write(const std::string& path, char* offset, size_t len){
	if(path != stdio_path){
		try{
			_fpdict.at(path).write(offset, len);
		}catch(const std::out_of_range& e){
			throw FileNotOpenedError
				("Can't write to file " + path + ", it is not opened");
		}
	}
	else{
		throw FileModeIncompatibleError
			("Can't write binary data to stdio_path" );
	}
}

std::iostream& iomgr_t::at(const std::string& path){
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

// helper
bool iomgr_t::path_exist(const std::string& path){
	return (_fpdict.find(path) != _fpdict.end());
}

bool iomgr_t::is_stdio(const std::string& path){
	return(path == io::stdio_path);
}

int main(void){
	using namespace std;
	io::iomgr_t iomgr;
	iomgr.at("__stdio__") << "hjahalsdfjadsl";
	double a[] = {1.0};
	return 0;
}
