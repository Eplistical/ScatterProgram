#include <string>
#include <vector>
#include <stdexcept>
#include "io_var.hpp"
#include "rem.hpp"
#include "iomgr.hpp"
#include "fileio.hpp"

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

void rwinit(scatter::initstate_t& initstate, char op){
	// construct header
	std::vector<double> header;
	header.push_back(dim);
	header.insert(header.end(), mass.begin(), mass.end());
	header.push_back(initstate.get_temp());
	// construct/get data
	const size_t datalen = dim * Ntraj * 2;
	std::vector<double>& rp = initstate.get_rp_ref();
	rp.resize(datalen);
	// read/write
	rwfile( filemgr, 
			initfile, 
			header, 
			rp,
			op);
}

void rwdat(const scatter::surf_t& surf, scatter::grid_t& grid, char op){
	const size_t surf_num = surf.get_surf_number();
	// construct header
	std::vector<double> header;
	std::vector<double> tmp;
	// dimension info
	header.push_back(dim);
	header.push_back(surf_num);
	// system info
	header.push_back(kT);
	header.push_back(Gamma0);
	header.insert(header.end(), mass.begin(), mass.end());
	// surf para info
	for(size_t d = 0; d < dim; d++){
		tmp = surf.get_gamma_para(d);
		header.insert(header.end(), tmp.begin(), tmp.end());
	}
	for(size_t i = 0; i < surf_num; i++){
		for(size_t d = 0; d < dim; d++){
			tmp = surf.get_energy_para(i, d);
			header.insert(header.end(), tmp.begin(), tmp.end());
		}
	}
	// grid info
	tmp = grid.get_rmin();
	header.insert(header.end(), tmp.begin(), tmp.end());
	tmp = grid.get_rmax();
	header.insert(header.end(), tmp.begin(), tmp.end());
	tmp = grid.get_dr();
	header.insert(header.end(), tmp.begin(), tmp.end());
	// construct/get data
	const size_t datalen = grid.get_forcelen() + grid.get_efriclen() + grid.get_fBCMElen();
	std::vector<double>& fef = grid.get_fef_ref();
	fef.resize(datalen);
	// read/write
	rwfile( filemgr, 
			initfile, 
			header, 
			fef,
			op);
}

// API
void scatter::io::savedat(const scatter::surf_t& surf, scatter::grid_t& grid){
    IO::info("saving data to " + scatter::io::datfile + " ...  ", true);
    rwdat(surf, grid, 'w');
    IO::info("done");
}

void scatter::io::loaddat(const scatter::surf_t& surf, scatter::grid_t& grid){
    IO::info("loading data from " + scatter::io::datfile + " ...  ", true);
    rwdat(surf, grid, 'r');
    IO::info("done");
}

void scatter::io::saveinit(scatter::initstate_t& initstate){
    IO::info("saving r0p0 to " + scatter::io::initfile + " ...  ", true);
    rwinit(initstate, 'w');
    IO::info("done");
}

void scatter::io::loadinit(scatter::initstate_t& initstate){
    IO::info("loading r0p0 to " + scatter::io::initfile + " ...  ", true);
    rwinit(initstate, 'r');
    IO::info("done");
}

void scatter::io::loadinfile(void){
    IO::info("loading infile: " + scatter::io::infile + " ...  ", true);
    rinfile();
    IO::info("done");
}

void scatter::io::print_file(std::string file){
    // copy infile content to output
    IO::info("infile:");
    IO::drawline('-');
    IO::drawline('-');
}
