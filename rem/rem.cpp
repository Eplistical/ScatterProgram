#include <string>
#include "rem.hpp"
// variables in scatter::rem


// job control parameters
const std::string& scatter::rem::jobname = "test";
const bool& scatter::rem::loaddat = true;
const size_t& scatter::rem::threadNum = 1;

// basic parameters for the system(useful both for structure & dynamics)
const size_t& scatter::rem::dim = 1;
const size_t& scatter::rem::dim2 = scatter::rem::dim * scatter::rem::dim;
const double& scatter::rem::hbar = 1.0;
const double& scatter::rem::kT = 0;
const double& scatter::rem::Gamma0 = 0;
