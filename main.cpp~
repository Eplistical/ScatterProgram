#include"header.hpp"
#include"global.hpp"
#include"VEC.hpp"
#include"Parameter.hpp"
#include"surfaces.hpp"
#include"CME.hpp"
#include"EF.hpp"
#include"anal.hpp"
#include"generic.hpp"
#include"filemgr.hpp"
#include"iomgr.hpp"
#include"timer.hpp"
#include"initstate.hpp"

void initstate(const Parameter& para, std::vector<Particle>& swarm) NOEXCEPT{
	for(int traj = 0; traj < para.Ntraj; traj++){
		// r, p
		swarm[traj].r.clear();
		swarm[traj].p.clear();
		for(int d = 0;d < DIM;d++){
			swarm[traj].r.push_back(para.r0p0[d + traj * DIM * 2]);
			swarm[traj].p.push_back(para.r0p0[d + DIM + traj * DIM * 2]);
		}
		// ranforce
		swarm[traj].ranforce.assign(DIM, 0.0);
		// surf
		swarm[traj].surf = 0;
	}
}

void runsimu(Parameter& para) NOEXCEPT{
	const size_t record_len = para.Nstep / para.Anastep;
	std::vector<Particle> CMEswarm(para.Ntraj);
	std::vector<Particle> BCMEswarm(para.Ntraj);
	std::vector<Particle> EFswarm(para.Ntraj);
	std::vector<Particle> CMErecord(record_len * para.Ntraj);
	std::vector<Particle> BCMErecord(record_len * para.Ntraj);
	std::vector<Particle> EFrecord(record_len * para.Ntraj);
	// init
	CME_init();
	para.prepdat();
	// get initial state
	initializer::generate_swarm(para, CMEswarm, 0);
	initializer::generate_swarm(para, BCMEswarm, 0);
	initializer::generate_swarm(para, EFswarm, 0);
	/*
	   initstate(para, CMEswarm);
	   initstate(para, BCMEswarm);
	   initstate(para, EFswarm);
	   */
	// run simulation
	size_t step, traj;
	size_t irecord;
#pragma omp parallel for if(threadNum > 1)  \
	default(shared)                     \
	private(traj, step, irecord)        \
	schedule(dynamic)
	for(traj = 0; traj < para.Ntraj; traj++){
		step = 0;
		irecord = 0;
		while(step < para.Nstep){
			// store anastep data
			if(step % para.Anastep == 0){
				CMErecord[traj + irecord * para.Ntraj] = CMEswarm[traj];
				BCMErecord[traj + irecord * para.Ntraj] = BCMEswarm[traj];
				EFrecord[traj + irecord * para.Ntraj] = EFswarm[traj];
				irecord++;
			}
			// evolve
			CME(para, CMEswarm[traj]);
			bCME(para, BCMEswarm[traj]);
			EF(para, EFswarm[traj]);
			step++;
		}
	}
	// output
	IO::info("simulation:");
	IO::tab("0,1,2,3,4,5,6", ',');
	IO::tab("step, <x>, <z>, <Ekx>, <Ekz>, <n1>, <nvib>", ',');
	IO::drawline('#');
	for(irecord = 0;irecord < record_len;irecord++){
		// analyze
		IO::tab(irecord * para.Anastep * para.dt); 
		anal(para, &CMErecord[irecord * para.Ntraj], MODE::DEFAULT_CME);
		anal(para, &BCMErecord[irecord * para.Ntraj], MODE::DEFAULT_BCME);
		anal(para, &EFrecord[irecord * para.Ntraj], MODE::DEFAULT_EF);
		IO::newline();
	}
	IO::drawline('#');
	anal(para, &CMErecord[(record_len - 1) * para.Ntraj], MODE::VIB_DIST_CME);
	anal(para, &CMErecord[(record_len - 1) * para.Ntraj], MODE::RP_DIST);
	anal(para, &BCMErecord[(record_len - 1) * para.Ntraj], MODE::VIB_DIST_BCME);
	anal(para, &BCMErecord[(record_len - 1) * para.Ntraj], MODE::RP_DIST);
	anal(para, &EFrecord[(record_len - 1) * para.Ntraj], MODE::VIB_DIST_EF);
	anal(para, &EFrecord[(record_len - 1) * para.Ntraj], MODE::RP_DIST);
	IO::info("CME_hopper_counter: ");
	IO::keyval("0->1",CME_hopper_counter[0]);
	IO::keyval("1->0",CME_hopper_counter[1]);
	IO::info("BCME_hopper_counter: ");
	IO::keyval("0->1",BCME_hopper_counter[0]);
	IO::keyval("1->0",BCME_hopper_counter[1]);
}

bool stopnow(const Parameter& para, const Particle * const m) NOEXCEPT{
	return true;
}

void test(Parameter& para) NOEXCEPT{
	// show UMPF & BCME surfaces
	//para.loadr0p0();
	para.prepdat();
	VEC r(DIM), r0(DIM);
	double UMPF;
	double UBCME[DIM];
	VEC f;

	IO::info("surfaces:");
	IO::drawline('#');
	//for(size_t jx = 0;jx < para.Nr[0];jx++){
	for(size_t jx = 0;jx < para.Nr[0];jx++){
		r[0] = para.rmin[0] + para.dr[0] * jx; 
		//r[0] = 0.0;
		//r[1] = para.rmin[1];
		r[1] = -1.8;
		// calc for z = zmin
		if(jx == 0){
			UMPF = 0.0;
			UBCME[0] = 0.0;
			UBCME[1] = 0.0;
		}
		else{
			r0 = r;
			r0[0] -= para.dr[0];
			// UMPF
			f = (para.get_force(r) + para.get_force(r0)) / 2;
			UMPF -= f[0] * para.dr[0];
			// UBCME
			f = (para.get_fBCME(r) + para.get_fBCME(r0)) / 2;
			UBCME[0] -= ( (fF0(r) + fF0(r0)) / 2 + f )[0] * para.dr[0];
			UBCME[1] -= ( (fF1(r) + fF1(r0)) / 2 + f )[0] * para.dr[0];
		}
		// output
		IO::tab(r[0]);
		IO::tab(r[1]);
		IO::tab(fU0(r));
		IO::tab(fU1(r));
		IO::tab(UBCME[0]);
		IO::tab(UBCME[1]);
		IO::tab(UMPF);
		IO::tab(fGamma(r, para.Gamma));
		IO::newline();
		continue;
		// calc for other z's
		for(size_t jz = 1;jz < para.Nr[1];jz++){
			r[1] = para.rmin[1] + para.dr[1] * jz;
			r0 = r;
			r0[1] -= para.dr[1];
			// UMPF
			f = (para.get_force(r) + para.get_force(r0)) / 2;
			UMPF -= f[1] * para.dr[1];
			// UBCME
			f = (para.get_fBCME(r) + para.get_fBCME(r0)) / 2;
			UBCME[0] -= ( (fF0(r) + fF0(r0)) / 2 + f )[1] * para.dr[1];
			UBCME[1] -= ( (fF1(r) + fF1(r0)) / 2 + f )[1] * para.dr[1];
			// output
			IO::tab(r[0]);
			IO::tab(r[1]);
			IO::tab(fU0(r));
			IO::tab(fU1(r));
			IO::tab(UBCME[0]);
			IO::tab(UBCME[1]);
			IO::tab(UMPF);
			IO::tab(fGamma(r, para.Gamma));
			IO::newline();
		}
	}
	IO::drawline('#');
}

int main(int argc, char* argv[]){
	size_t j, t_start, t_end;
	// parse arguments
	parse_args(argc, argv);
	// environment
	srand(time(NULL));
	cout.precision(8);
	mkl_set_num_threads(threadNum);
	omp_set_num_threads(threadNum);
#ifdef _DEBUG
	IO::info("_DEBUG defined");
#endif
	Timer::now();
	IO::keyval("threadNum", threadNum, true);
	IO::keyval("runmode", runmode, true);
	IO::keyval("infile", infile, true);
	IO::keyval("loaddat", loaddat, true);
	// load para
	Parameter para(infile, runmode);
	para.showpara();

	// run program
	Timer::tic();
	switch(runmode){
		case RUNMODE::TEST :
			test(para);
			break;
		case RUNMODE::PREP : 
			para.prepr0p0();
			break;
		case RUNMODE::SIMULATION : 
			para.loadr0p0();
			runsimu(para);
			break;
		case RUNMODE::DAT : 
			loaddat = 0;
			para.prepdat();
			FileMgr::savedat(para);
			break;
		case RUNMODE::SURF : 
			shape(para, 0);
			shape(para, 1);
			xzshape(para);
			break;
		default : 
			throw std::out_of_range("Unknown mode");
			break;
	}
	// ending
	IO::info("Program Ends");
	Timer::toc();
	Timer::now();
	return 0;
}
