#ifndef _SURFACES_TYPES_HPP
#define _SURFACES_TYPES_HPP

#include <vector>
#include <memory>
#include <map>
#include "surfmode.hpp"

#include "constant.hpp"
#include "step.hpp"
#include "harmonic.hpp"
#include "fermi.hpp"
#include "gaussian.hpp"
#include "morse.hpp"
#include "newns.hpp"

namespace surfaces_collection {
	class single_surf_t{
		private:
			std::vector<std::shared_ptr<basesurf>> surfptr;
		public:
			// --- setters --- //
			void append_dim(enumspace::surfmode_enum mode, std::vector<double> para){
				/* Append a dimension to current surface
				 *
				 * param mode: surface mode
				 * param para: vector containing enough parameters for the mode
				 */
				using enumspace::surfmode_enum;
				switch(mode){
					case surfmode_enum::CONSTANT:
						surfptr.push_back(std::make_shared<constant>(para)); break;
					case surfmode_enum::STEP:
						surfptr.push_back(std::make_shared<step>(para)); break;
					case surfmode_enum::HARMONIC:
						surfptr.push_back(std::make_shared<harmonic>(para)); break;
					case surfmode_enum::FERMI:
						surfptr.push_back(std::make_shared<fermi>(para)); break;
					case surfmode_enum::GAUSSIAN:
						surfptr.push_back(std::make_shared<gaussian>(para)); break;
					case surfmode_enum::MORSE:
						surfptr.push_back(std::make_shared<morse>(para)); break;
					case surfmode_enum::NEWNS:
						surfptr.push_back(std::make_shared<newns>(para)); break;
				}
			}

			void clear(void)
			{ 
				surfptr.clear(); 
			}

			// --- getters --- //
			
			// dimension num
			size_t dim(void) const 
			{ 
				return surfptr.size(); 
			}

			// mode for d^th dimension
			enumspace::surfmode_enum get_mode(int d) const 
			{ 
				return enumspace::surfmode_dict.left.at(surfptr.at(d)->get_mode()); 
			}

			// para number for d^th dimension
			size_t get_Npara(int d) const 
			{ 
				return surfptr.at(d)->get_Npara();
			}

			// para for d^th dimension
			std::vector<double> get_para(int d) const
			{
				return surfptr.at(d)->get_para(); 
			}

			// string for all dimensions
			std::string get_expr(void) const
			{
				std::stringstream expr;
				for(size_t d = 0; d < dim(); ++d){
					expr << "dim " << d << ":" << std::endl;
					expr << surfptr.at(d)->get_expr() << std::endl;
				}
				return expr.str();
			}

			// U(r) for given dimension
			double get_U(int d, double x) const
			{
				return surfptr[d]->U(x);
			}
			// U(r) as a vector
			std::vector<double> get_U(const std::vector<double>& r) const
			{
				std::vector<double> rst(dim());
				for(size_t d = 0, N = dim() ; d < N; ++d){
					rst[d] = get_U(d, r[d]);
				}
				return rst;
			}

			// U'(r) for given dimension
			double get_dUdx(int d, double x) const{
				return surfptr[d]->dUdx(x);
			}
			// U'(r) as a vector
			std::vector<double> get_dUdx(const std::vector<double>& r) const
			{
				std::vector<double> rst(dim());
				for(size_t d = 0, N = dim() ; d < N; ++d){
					rst[d] = get_dUdx(d, r[d]);
				}
				return rst;
			}
	};
};

#endif
