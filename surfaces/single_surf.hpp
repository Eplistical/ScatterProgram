#ifndef _SINGLE_SURF_HPP
#define _SINGLE_SURF_HPP

#include <vector>
#include <memory>
#include <sstream>
#include "types.hpp"
#include "packages.hpp"

namespace surfaces{
    class single_surf_t{
        private:
            size_t dim;
            std::vector<_1d_surf_ptr_t> surfptr;
            std::vector<surf_info_t> para_list;
        public:
            // constructor & initializer
            single_surf_t(){};
            explicit single_surf_t(size_t N){ 
                dim = N;
                surfptr.resize(dim);
                para_list.resize(dim);
            }

            explicit single_surf_t(const std::vector<surf_info_t>& para){
                dim = para.size();
                surfptr.resize(dim);
                para_list.resize(dim);
                init_surfptr(para);
            }

            void init_surfptr(const std::vector<surf_info_t>& para){
                for(int i = 0; i < dim; i++){
                    switch(para.at(i).mode){
                        case surfaces::packages::CONSTANT:
                            surfptr.at(i) = _1d_surf_ptr_t(new surfaces::packages::constant(para.at(i).para));
                            break;
                        case surfaces::packages::STEP:
                            surfptr.at(i) = _1d_surf_ptr_t(new surfaces::packages::step(para.at(i).para));
                            break;
                        case surfaces::packages::HARMONIC:
                            surfptr.at(i) = _1d_surf_ptr_t(new surfaces::packages::harmonic(para.at(i).para));
                            break;
                        case surfaces::packages::FERMI:
                            surfptr.at(i) = _1d_surf_ptr_t(new surfaces::packages::fermi(para.at(i).para));
                            break;
                        case surfaces::packages::GAUSSIAN:
                            surfptr.at(i) = _1d_surf_ptr_t(new surfaces::packages::gaussian(para.at(i).para));
                            break;
                        case surfaces::packages::MORSE:
                            surfptr.at(i) = _1d_surf_ptr_t(new surfaces::packages::morse(para.at(i).para));
                            break;
                        case surfaces::packages::NEWNS:
                            surfptr.at(i) = _1d_surf_ptr_t(new surfaces::packages::newns(para.at(i).para));
                            break;
                    }
                    para_list.at(i) = para.at(i);
                    para_list.at(i).para.resize(surfptr.at(i)->N());
                }
            }

            // getter
            std::vector<surf_info_t> get_paralist(void) const{
				return para_list; 
			}

            surf_info_t get_paralist(int d) const{ 
				return para_list.at(d); 
			}

            std::vector<double> get_para_as_vec(int d) const{ 
				return para_list.at(d).to_vec(); 
			}

            size_t get_dim(void) const{
				return dim; 
			}

            _1d_surf_ptr_t get_surfdim(int d) const{
				return surfptr.at(d); 
			}

            std::string get_expr(void) const{
                std::stringstream expr;
                for(size_t d = 0; d < dim; d++){
                    expr << "dim " << d << ":" << std::endl;
                    expr << surfptr.at(d)->get_expr() << std::endl;
                }
                return expr.str();
            }

            // calc U(r)
            std::vector<double> get_U(const std::vector<double>& r) const{
                std::vector<double> rst(dim);
                for(size_t i = 0 ; i < dim; i++){
                    rst.at(i) = surfptr.at(i)->U(r.at(i));
                }
                return rst;
            }

            // calc U'(r)
            std::vector<double> get_dUdx(const std::vector<double>& r) const{
                std::vector<double> rst(dim);
                for(size_t i = 0; i < dim; i++){
                    rst.at(i) = surfptr.at(i)->dUdx(r.at(i));
                }
                return rst;
            }
    };
};

#endif
