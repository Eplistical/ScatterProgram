#ifndef _IOER_OUTPUT_T_HPP
#define _IOER_OUTPUT_T_HPP
/* module for output utilities in ioer
 *
 * Gaohan
 */
#include <iostream>
#include <iomanip>
#include <vector>
#include "type_traiter.hpp"
#include "ioer_macros.hpp"
#include "io_base_t.hpp"
#include "ioer_var.hpp"
#include "ioer_exceptions.hpp"

namespace ioer {
	using namespace std;
	using namespace type_traiter;
	using ioer::io_base_obj;

	class _pair_output_functor_t {
		private:
			string _path; 
			string _dlm;
			size_t _width;
			bool _keyfirst;
			bool _nonewline;
		public:
			explicit _pair_output_functor_t(const string& path, const string& dlm, size_t width, bool keyfirst, bool nonewline) 
				: _path(path), _dlm(dlm), _width(width), _keyfirst(keyfirst), _nonewline(nonewline) {}
			~_pair_output_functor_t() = default;

			// no copy or assign
			_pair_output_functor_t(const _pair_output_functor_t&) = delete;
			_pair_output_functor_t& operator=(const _pair_output_functor_t&) = delete;

			// setter
			void config(const string& path, const string& dlm, size_t width, bool keyfirst, bool nonewline) {
				_path = path;
				_dlm = dlm;
				_width = width;
				_keyfirst = keyfirst;
				_nonewline = nonewline;
			}

			// operator()
			template<typename KeyType, typename ValType>
				typename enable_if< is_direct_outputable< ValType >::value, const _pair_output_functor_t&>::type
				operator()(const KeyType& key, const ValType& val) const{
					if(_keyfirst) 
						io_base_obj[_path] <<  setw(_width) << key << _dlm << setw(_width) << val;
					else 
						io_base_obj[_path] <<  setw(_width) << val << _dlm << setw(_width) << key;
					if(!_nonewline) io_base_obj[_path] <<  "\n";
					return *this;
				}

			template<typename KeyType, typename ValType>
				typename enable_if< is_sequence_container<ValType>::value, const _pair_output_functor_t& >::type
				operator()(const KeyType& key, const ValType& val) const{
					if(_keyfirst) {
						io_base_obj[_path] <<  setw(_width) << key << _dlm;
						for(auto& it : val) io_base_obj[_path] <<  setw(_width) << it;
					}
					else{
						for(auto& it : val) io_base_obj[_path] <<  setw(_width) << it;
						io_base_obj[_path] <<  _dlm << setw(_width) << key ;
					}
					if(!_nonewline) io_base_obj[_path] <<  "\n";
					return *this;
				}
	}; // class _pair_output_functor_t


	class output_t {
		protected:
			const string _path;

			size_t _width = 16;
			string _dlm = " ";

			// functors
			_pair_output_functor_t _pair_output_functor_obj;

		public:
			explicit output_t(const string& path = STDIO_PATH, ios::openmode mode = ios::out)
				: 	_path(path), _pair_output_functor_obj(STDIO_PATH, _dlm, _width, true, false)
			{ 
				io_base_obj.open(path, mode | ios::out);
			}
			virtual ~output_t() = default;

			// no copy or assign
			output_t(const output_t&) = delete;
			output_t& operator=(const output_t&) = delete;

			// getter
			size_t width(void) { return _width; }
			string dlm(void) { return _dlm; }

			// setter
			size_t set_width(size_t width) { _width = width; }
			string set_dlm(const string& dlm) { _dlm = dlm; }

			// utilities

			// -- basic -- //
			void newline(void) {  io_base_obj.at(_path) << "\n"; }
			void drawline(char c, size_t len = 32) { io_base_obj.at(_path) << string(len, c) << "\n"; }

			// -- info -- //
			template<typename ParamType>
				typename enable_if< is_direct_outputable<ParamType>::value, void>::type
				_info(const ParamType& x) 
				{
					io_base_obj.at(_path) << x;
				}

			template<typename ParamType>
				typename enable_if< is_sequence_container<ParamType>::value, void>::type
				_info(const ParamType& x) 
				{
					for (auto& xi : x) {
						io_base_obj.at(_path) << xi << " ";
					}
				}

			template<typename ParamType, typename ... Types>
				void _info(const ParamType& x, const Types& ... otherx) 
				{
					_info(x);
					_info(otherx ...);
				}

			template<typename ... Types>
				void info_nonewline(const Types& ... x) 
				{
					_info(x ...);
				}

			template<typename ... Types>
				void info(const Types& ... x) 
				{
					info_nonewline(x ...);
					newline();
				}

			// -- tabout -- //
			template<typename ParamType>
				typename enable_if< is_direct_outputable<ParamType>::value, void>::type
				_tabout(const ParamType& x) 
				{
					io_base_obj.at(_path) << setw(_width) << x;
				}

			template<typename ParamType>
				typename enable_if< is_sequence_container<ParamType>::value, void>::type
				_tabout(const ParamType& x) 
				{
					for (auto& xi : x) {
						io_base_obj.at(_path) << setw(_width) << xi;
					}
				}

			template<typename ParamType, typename ... Types>
				void _tabout(const ParamType& x, const Types& ... otherx) 
				{
					_tabout(x);
					_tabout(otherx ...);
				}

			template<typename ... Types>
				void tabout_nonewline(const Types& ... x) 
				{
					_tabout(x ...);
				}

			template<typename ... Types>
				void tabout(const Types& ... x) 
				{
					tabout_nonewline(x ...);
					newline();
				}

			// -- pair output -- //
			const _pair_output_functor_t&
				keyval(bool _keyfirst = true, bool _nonewline = false)
				{
					_pair_output_functor_obj.config(_path, _dlm, _width, _keyfirst, _nonewline); 
					return _pair_output_functor_obj;
				}

			// -- binary output -- //
			template<typename ParamType>
				typename enable_if<is_fundamental<ParamType>::value, void>::type
				_write(const ParamType& x)
				{
					const char* offset = reinterpret_cast<const char*>(&x);
					io_base_obj.at(_path).write(offset, sizeof(x) * 8);
				}

			template<typename ParamType>
				typename enable_if<is_complex<ParamType>::value, void>::type
				_write(const ParamType& x)
				{
					using ValType = typename ParamType::value_type;
					ValType tmp[] = {x.real(), x.imag()};
					io_base_obj.at(_path).write
						(reinterpret_cast<const char*>(tmp), 2 * sizeof(ValType) * 8);
				}

			template<typename ParamType>
				typename enable_if<is_c_string<ParamType>::value, void>::type
				_write(const ParamType& x)
				{
					const char* cp = x;
					size_t N = 0;
					while(*cp != '\0') ++cp;
					io_base_obj.at(_path).write(cp, N * sizeof(char) * 8);
				}

			template<typename ParamType>
				typename enable_if<	is_array_container<ParamType>::value ||
				is_vector<ParamType>::value ||
				is_string<ParamType>::value,
				void>::type
					_write(const ParamType& x)
					{
						const char* offset = reinterpret_cast<const char*>(x.data());
						io_base_obj.at(_path).write(offset, x.size() * sizeof(typename ParamType::value_type) * 8);
					}

			template<typename ParamType>
				typename enable_if<	is_deque<ParamType>::value ||
				is_forward_list<ParamType>::value ||
				is_list<ParamType>::value
				, void>::type
				_write(const ParamType& x)
				{
					for(auto& it : x) {
						io_base_obj.at(_path).write
							(reinterpret_cast<const char*>(&x), sizeof(typename ParamType::value_type) * 8);
					}
				}


			template<typename ParamType, typename ... Types>
				void _write(const ParamType& x, const Types& ... otherx) 
				{
					_write(x);
					_write(otherx ...);
				}

			template<typename ... Types>
				void write(const Types& ... x) 
				{
					_write(x ...);
				}
	}; // class output_t

	// non-class functions for standard output
	static output_t STDOUT;

	inline void newline(void) 
	{
		STDOUT.newline();
	}

	inline void drawline(char c, size_t len = 32)
	{
		STDOUT.drawline(c, len);
	}

	template<typename ... Types>
		inline void info_nonewline(const Types& ... x) 
		{
			STDOUT.info_nonewline(x ...);
		}
	template<typename ... Types>
		inline void info(const Types& ... x) 
		{
			STDOUT.info(x ...);
		}
	template<typename ... Types>
		inline void tabout(const Types& ... x) 
		{
			STDOUT.tabout(x ...);
		}
	template<typename ... Types>
		inline void tabout_nonewline(const Types& ... x) 
		{
			STDOUT.tabout_nonewline(x ...);
		}
	template<typename ... Types>
		inline void write(const Types& ... x) 
		{
			STDOUT.write(x ...);
		}
	inline const _pair_output_functor_t&
		keyval(bool _keyfirst = true, bool _nonewline = false)
		{
			return STDOUT.keyval(_keyfirst, _nonewline);
		}
};
#endif
