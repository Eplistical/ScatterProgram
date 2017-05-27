#ifndef _SCATTER_INITSTATE_T_HPP
#define _SCATTER_INITSTATE_T_HPP
// module for initstate in simulation
#include <vector>
#include <random>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include "rem.hpp"

namespace scatter{
	namespace mode{
		enum INIT_MODE{DELTA, RING, RP_GAUSSIAN, P_GAUSSIAN};
		const boost::bimap<std::string, int> init_mode_dict = 
			boost::assign::list_of<boost::bimap<std::string, int>::relation>
			("delta", INIT_MODE::DELTA)
			("ring", INIT_MODE::RING)
			("rp_gaussian", INIT_MODE::RP_GAUSSIAN)
			("p_gaussian", INIT_MODE::P_GAUSSIAN)
			;
	};

	class initstate_t{
		private:
			size_t _vibstate;
			size_t _elestate;
			double _temp;
			std::vector<int> _mode;
			std::vector<double> _ravg;
			std::vector<double> _pavg;
			std::vector<double> _rp;
		public:
			// constructor
			initstate_t(void);
			initstate_t(size_t vibstate, size_t elestate, double temp);
			// getter & setter
			std::vector<double>& get_rp_ref(void);
			// setter
			void set_mode(const std::vector<int>& mode);
			void set_mode(size_t d, int mode);
			void set_ravg(const std::vector<double>& ravg);
			void set_ravg(size_t d, double ravg);
			void set_pavg(const std::vector<double>& pavg);
			void set_pavg(size_t d, double pavg);
			void generate_rp(void);
			// getter
			size_t get_vibstate(void) const;
			size_t get_elestate(void) const;
			double get_temp(void) const;
			int get_mode(size_t d) const;
			double get_ravg(size_t d) const;
			double get_pavg(size_t d) const;
	};
};
#endif
