#include <string>
#include "filemgr/filemgr.hpp"
#include "io_var.hpp"

const std::string& scatter::io::infile;
const std::string& scatter::io::datfile;
const std::string& scatter::io::initfile;
const std::string& scatter::io::outfile;

iomgr::filemgr_t scatter::io::filemgr;
