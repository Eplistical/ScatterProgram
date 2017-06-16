#ifndef _IOER_FUNCTORS_HPP
#define _IOER_FUNCTORS_HPP
/* functors used in ioer */

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>

namespace ioer{
	class keyval_functor{
		private:
			const size_t _width;
			const bool _keyfirst;
			const std::string _dlm;
			std::ostream& _dest; 
		public:
			explicit keyval_functor(std::ostream& dest, const std::string& dlm, const size_t width, const bool keyfirst) :
				_dest(dest), 
				_dlm(dlm),
			   	_width(width), 
				_keyfirst(keyfirst){}

			template<typename KeyType, typename ValType>
				const keyval_functor& operator()(const KeyType& key, const ValType& val, bool newline = true) const{
					std::ostringstream buf;
					if(_keyfirst) buf <<  std::setw(_width) << key << _dlm << std::setw(_width) << val;
					else buf <<  std::setw(_width) << val << _dlm << std::setw(_width) << key;
					if(newline) buf <<  "\n";
					_dest << buf.str();
					return *this;
				}

			template<typename KeyType, typename ValType>
				const keyval_functor& operator()(const KeyType& key, const std::vector<ValType>& val, bool newline = true) const{
					std::ostringstream buf;
					if(_keyfirst){
						buf <<  std::setw(_width) << key << _dlm;
						for(auto& it : val) buf <<  std::setw(_width) << it;
					}
					else{
						for(auto& it : val) buf <<  std::setw(_width) << it;
						buf <<  _dlm << std::setw(_width) << key ;
					}
					if(newline) buf <<  "\n";
					_dest << buf.str();
					return *this;
				}
		};
};
#endif
