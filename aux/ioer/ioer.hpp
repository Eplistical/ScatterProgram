#ifndef _IOER_HPP
#define _IOER_HPP
/* module for IOer 
 * written based on <iostream> & operator()
 *
 * Gaohan
 */
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include "type_traiter.hpp"

namespace ioer{
	using namespace std;
	using namespace type_traiter;


	


	inline void newline(void) { cout << endl; }
	inline void drawline(char c, size_t len = 32) { cout << string(len, c) << endl; }

	// -- tabout -- //
	template<typename ParamType>
		inline typename enable_if< !is_vector<ParamType>::value, void>::type
		_tabout(ParamType x) {
			cout << std::setw(16) << x;
		}

	template<typename ParamType>
		inline typename enable_if< is_vector<ParamType>::value, void>::type
		_tabout(ParamType x) {
			for (auto& xi : x) {
				cout << std::setw(16) << xi;
			}
		}

	template<typename ParamType, typename ... Types>
		inline void tabout_nonewline(ParamType x, Types ... otherx) {
			_tabout(x);
			_tabout(otherx ...);
		}

	template<typename ParamType, typename ... Types>
		inline void tabout(ParamType x, Types ... otherx) {
			tabout_nonewline(x, otherx ...);
			newline();
		}


	// -- info -- //
	template<typename ParamType>
		inline typename enable_if< !is_vector<ParamType>::value, void>::type
		_info(ParamType x) {
			cout << setw(16) << x;
		}

	template<typename ParamType>
		inline typename enable_if< is_vector<ParamType>::value, void>::type
		_info(ParamType x) {
			for (auto& xi : x) {
				cout << setw(16) << xi;
			}
		}

	template<typename ParamType, typename ... Types>
		inline void _info(ParamType x, Types ... otherx) {
			_info(x);
			_info(otherx ...);
		}

	template<typename ... Types>
		inline void info(Types ... x) {
			_info(x ...);
			newline();
		}

	template<typename ... Types>
		inline void info_nonewline(Types ... x) {
			_info(x ...);
		}

	class keyval{
		private:
			const size_t _width;
			const bool _keyfirst;
			const std::string _dlm;
			std::ostream& _dest; 
		public:
			explicit keyval(std::ostream& dest = std::cout, const std::string& dlm = " : ") 
				:_dest(dest), _dlm(dlm), _width(16), _keyfirst(true) {}

			template<typename KeyType, typename ValType>
				const keyval& operator()(const KeyType& key, const ValType& val, bool newline = true) const{
					std::ostringstream buf;
					if(_keyfirst) buf <<  setw(_width) << key << _dlm << setw(_width) << val;
					else buf <<  setw(_width) << val << _dlm << setw(_width) << key;
					if(newline) buf <<  "\n";
					_dest << buf.str();
					return *this;
				}

			template<typename KeyType, typename ValType>
				const keyval& operator()(const KeyType& key, const std::vector<ValType>& val, bool newline = true) const{
					std::ostringstream buf;
					if(_keyfirst) {
						buf <<  setw(_width) << key << _dlm;
						for(auto& it : val) buf <<  setw(_width) << it;
					}
					else{
						for(auto& it : val) buf <<  setw(_width) << it;
						buf <<  _dlm << setw(_width) << key ;
					}
					if(newline) buf <<  "\n";
					_dest << buf.str();
					return *this;
				}
	};
};
#endif
