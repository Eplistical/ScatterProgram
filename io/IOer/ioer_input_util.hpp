#ifndef _IOER_INPUT_T_HPP
#define _IOER_INPUT_T_HPP
/* module for input utilities in ioer
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

	class input_t {
		protected:
			const string _path;

			size_t _width = 16;
			string _dlm = " ";

		public:
			explicit input_t(const string& path = STDIO_PATH, ios::openmode mode = ios::in)
				: 	_path(path)
			{ 
				io_base_obj.open(path, mode | ios::in);
			}
			virtual ~input_t() = default;

			// no copy or assign
			input_t(const input_t&) = delete;
			input_t& operator=(const input_t&) = delete;
			
			// getter
			size_t width(void) { return _width; }
			string dlm(void) { return _dlm; }

			// setter
			size_t set_width(size_t width) { _width = width; }
			string set_dlm(const string& dlm) { _dlm = dlm; }

			// open/close
			void open(const string& path, ios::openmode mode = ios::in) 
			{
				io_base_obj.open(path, mode);
			}
			void close(void) 
			{
				io_base_obj.close(_path);
			}

			// utilities

			// -- binary input -- //
			template<typename ParamType>
				typename enable_if<is_fundamental<ParamType>::value, void>::type
				_read(ParamType& x)
				{
					io_base_obj.at(_path).read
						(reinterpret_cast<char*>(&x), static_cast<std::streamsize>(sizeof(x)));
				}

			template<typename ParamType>
				typename enable_if<is_complex<ParamType>::value, void>::type
				_read(ParamType& x)
				{
					using ValType = typename ParamType::value_type;
					ValType tmp[2];
					io_base_obj.at(_path).read
						(reinterpret_cast<char*>(tmp), static_cast<std::streamsize>(2 * sizeof(ValType)));
					x.real(tmp[0]);
					x.imag(tmp[1]);
				}

			template<typename ParamType>
				typename enable_if<	is_array_container<ParamType>::value ||
				is_vector<ParamType>::value ||
				is_string<ParamType>::value,
				void>::type
					_read(ParamType& x)
					{
						io_base_obj.at(_path).read
							(reinterpret_cast<char*>(&x[0]), static_cast<std::streamsize>(x.size() * sizeof(typename ParamType::value_type)));
					}

			template<typename ParamType>
				typename enable_if<	is_deque<ParamType>::value ||
				is_forward_list<ParamType>::value ||
				is_list<ParamType>::value
				, void>::type
				_read(ParamType& x)
				{
					for(auto& it : x) {
						io_base_obj.at(_path).read
							(reinterpret_cast<char*>(&x), static_cast<std::streamsize>(sizeof(typename ParamType::value_type)));
					}
				}


			template<typename ParamType, typename ... Types>
				void _read(ParamType& x, Types& ... otherx) 
				{
					_read(x);
					_read(otherx ...);
				}

			template<typename ... Types>
				void read(Types& ... x) 
				{
					_read(x ...);
				}
	}; // class input_t

};
#endif
