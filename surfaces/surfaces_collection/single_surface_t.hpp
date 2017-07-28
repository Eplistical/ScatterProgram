#ifndef _SURFACES_TYPES_HPP
#define _SURFACES_TYPES_HPP
// class for a single surface
#include "scatter_basic.hpp"

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
			std::vector< std::shared_ptr<basesurf> > surfptr;
		public:
			// --- setters --- //
			VOID_T append_dim(enumspace::surfmode_enum mode, std::vector<DOUBLE_T> para){
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

			VOID_T clear(VOID_T) noexcept
			{ 
				surfptr.clear(); 
			}

			// --- getters --- //
			
			// dimension num
			UINT_T dim(VOID_T) const noexcept
			{ 
				return surfptr.size(); 
			}

			// mode for d^th dimension
			enumspace::surfmode_enum get_mode(INT_T d) const 
			{ 
				return enumspace::surfmode_dict.left.at(surfptr.at(d)->get_mode()); 
			}

			// para number for d^th dimension
			UINT_T get_Npara(INT_T d) const 
			{ 
				return surfptr.at(d)->get_Npara();
			}

			// para for d^th dimension
			std::vector<DOUBLE_T> get_para(INT_T d) const
			{
				return surfptr.at(d)->get_para(); 
			}

			// STRING_T for all dimensions
			STRING_T get_expr(VOID_T) const
			{
				std::stringstream expr;
				for(UINT_T d = 0; d < dim(); ++d){
					expr << "dim " << d << ":" << std::endl;
					expr << surfptr.at(d)->get_expr() << std::endl;
				}
				return expr.str();
			}

			// U(r) for given dimension
			DOUBLE_T get_U(INT_T d, DOUBLE_T x) const
			{
				return surfptr.at(d)->U(x);
			}

			// U(r) as a vector
			std::vector<DOUBLE_T> get_U(const std::vector<DOUBLE_T>& r) const
			{
				std::vector<DOUBLE_T> rst(dim());
				for(UINT_T d = 0, N = dim() ; d < N; ++d){
					rst[d] = get_U(d, r[d]);
				}
				return rst;
			}

			// U'(r) for given dimension
			DOUBLE_T get_dUdx(INT_T d, DOUBLE_T x) const{
				return surfptr.at(d)->dUdx(x);
			}

			// U'(r) as a vector
			std::vector<DOUBLE_T> get_dUdx(const std::vector<DOUBLE_T>& r) const
			{
				std::vector<DOUBLE_T> rst(dim());
				for(UINT_T d = 0, N = dim() ; d < N; ++d){
					rst[d] = get_dUdx(d, r[d]);
				}
				return rst;
			}
	};
};

#endif
