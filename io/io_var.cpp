#include <string>
#include "filemgr/filemgr.hpp"
#include "io_var.hpp"

const std::string& scatter::io::infile = "INVALID";
const std::string& scatter::io::datfile = "INVALID";
const std::string& scatter::io::initfile = "INVALID";
const std::string& scatter::io::outfile = "INVALID";

iomgr::filemgr_t scatter::io::filemgr;
