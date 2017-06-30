#include <string>
#include <vector>
#include <stdexcept>
#include "io_var.hpp"
#include "rem.hpp"
#include "fileio.hpp"
#include "ioer.hpp"
#include "simulation.hpp"
#include "run.hpp"

using namespace scatter::rem;
using namespace scatter::simulation;
using namespace scatter::io;
using iomgr::filemgr_t;

static void rinfile(){
}

static void rwfile( filemgr_t& filemgr,
					const std::string filename, 
					std::vector<double>& header,
					std::vector<double>& data,
					char op,
					bool head_only = false 
					){
    /* store/load a general file
	 *  header & data are expected to be well-sized */
	const size_t dsize = sizeof(double);
	// write
    if(op == 'w'){
		filemgr.open(filename, std::ios::out | std::ios::binary);
		filemgr.write(filename, reinterpret_cast<char*>(&header.at(0)), header.size() * dsize);
		if(not head_only){
			filemgr.write(filename, reinterpret_cast<char*>(&data.at(0)), data.size() * dsize);
		}
		filemgr.close(filename);
    }
	// read
    else if(op == 'r'){
		const size_t headerlen = header.size();
		filemgr.open(filename, std::ios::in | std::ios::binary);
		if(head_only){
			filemgr.read(filename, reinterpret_cast<char*>(&header.at(0)), headerlen * dsize);
		}
		else{
			// check header
			std::vector<double> tmp(headerlen);
			filemgr.read(filename, reinterpret_cast<char*>(&tmp.at(0)), headerlen * dsize);
			for(size_t i = 0; i < headerlen;i++){
				if(header.at(i) != tmp.at(i)){
					std::stringstream errinfo;
					errinfo 
						<< "rwfile: Incompatible header.at"
						<< "(" << i << "): " << tmp.at(i)
						<< " != " << header.at(i) << std::endl;
					throw std::invalid_argument(errinfo.str());
				}
			}
			// read file body
			const size_t datalen = data.size();
			filemgr.read(filename, reinterpret_cast<char*>(&data.at(0)), datalen * dsize);
			filemgr.close(filename);
    	}
	}
}

void rwinit(char op){
	using scatter::simulation::r0p0;
	// construct header
	std::vector<double> header;
	header.push_back(dim);
	header.insert(header.end(), mass.begin(), mass.end());
	header.push_back(inittemp);
	// construct/get data
	const size_t datalen = dim * Ntraj * 2;
	std::vector<double> rp = r0p0;
	rp.resize(datalen);
	// read/write
	rwfile( filemgr, 
			initfile, 
			header, 
			rp,
			op);
	if(op == 'r'){
		const_cast<std::vector<double>&>(r0p0) = rp;
	}
}

void rwdat(char op){
	using scatter::surfaces_obj;
	using scatter::grid_obj;
	using scatter::surfaces::surfnum;
	using scatter::simulation::mass;
	using scatter::rem::dim;
	using scatter::rem::kT;
	using scatter::rem::Gamma0;

	// construct header
	std::vector<double> header;
	std::vector<double> tmp;
	// dimension info
	header.push_back(dim);
	header.push_back(surfnum);
	// system info
	header.push_back(kT);
	header.push_back(Gamma0);
	header.insert(header.end(), mass.begin(), mass.end());
	// surfaces para info
	for(size_t d = 0; d < dim; d++){
		tmp = surfaces_obj.get_gamma_para(d);
		header.insert(header.end(), tmp.begin(), tmp.end());
	}
	for(size_t i = 0; i < surfnum; i++){
		for(size_t d = 0; d < dim; d++){
			tmp = surfaces_obj.get_energy_para(i, d);
			header.insert(header.end(), tmp.begin(), tmp.end());
		}
	}
	// grid info
	tmp = grid_obj.get_rmin();
	header.insert(header.end(), tmp.begin(), tmp.end());
	tmp = grid_obj.get_rmax();
	header.insert(header.end(), tmp.begin(), tmp.end());
	tmp = grid_obj.get_dr();
	header.insert(header.end(), tmp.begin(), tmp.end());
	// construct/get data
	const size_t datalen = grid_obj.get_forcelen() + grid_obj.get_efriclen() + grid_obj.get_fBCMElen();
	std::vector<double>& fef = grid_obj.get_fef_ref();
	fef.resize(datalen);
	// read/write
	rwfile( filemgr, 
			initfile, 
			header, 
			fef,
			op);
}

// API
void scatter::io::savedat(void){
    ioer::info_nonewline("saving data to " + scatter::io::datfile + " ...  ");
    rwdat('w');
    ioer::info_nonewline("done");
}

void scatter::io::loaddat(void){
    ioer::info_nonewline("loading data from " + scatter::io::datfile + " ...  ");
    rwdat('r');
    ioer::info("done");
}

void scatter::io::saveinit(void){
    ioer::info_nonewline("saving r0p0 to " + scatter::io::initfile + " ...  ");
    rwinit('w');
    ioer::info("done");
}

void scatter::io::loadinit(void){
    ioer::info_nonewline("loading r0p0 to " + scatter::io::initfile + " ...  ");
    rwinit('r');
    ioer::info("done");
}

void scatter::io::loadinfile(void){
    ioer::info_nonewline("loading infile: " + scatter::rem::infile + " ...  ", true);
    rinfile();
    ioer::info("done");
}

void scatter::io::print_file(std::string file){
    // copy infile content to output
    ioer::info("infile:");
    ioer::drawline('-');
    ioer::drawline('-');
}
