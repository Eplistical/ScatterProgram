#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "filemgr.hpp"

using namespace iomgr;
// constructor
filemgr_t::filemgr_t(){}
// getter
size_t filemgr_t::size(void){ return _fpdict.size(); }
// modifier
void filemgr_t::open(const std::string& name, std::fstream::openmode mode){
    assert(not name_exist(name));
    _fpdict.insert( std::pair<std::string, std::fstream>
                    (name, std::fstream(name, mode))
                    );
}
void filemgr_t::close(const std::string& name){
    _fpdict.at(name).close();
    _fpdict.erase(name);
}
// reader/writer for binary files
void filemgr_t::read(const std::string& name, char* offset, size_t len){
    _fpdict.at(name).read(offset, len);
}
void filemgr_t::write(const std::string& name, char* offset, size_t len){
    _fpdict.at(name).write(offset, len);
}
// reader/writer for text files
std::string filemgr_t::getline(const std::string& name){
    std::string rst;
    std::getline(_fpdict[name], rst);
    return rst;
}
void filemgr_t::newline(const std::string& name){
    _fpdict.at(name) << std::endl;
}
void filemgr_t::writeline(const std::string& name, const std::string& text){
    _fpdict.at(name) << text << std::endl;
}
void filemgr_t::drawline(const std::string& name, char c, size_t len){
    _fpdict.at(name) << std::string(len, c) << std::endl;
}
// helper
bool filemgr_t::name_exist(const std::string& name){
    return (_fpdict.find(name) != _fpdict.end());
}
