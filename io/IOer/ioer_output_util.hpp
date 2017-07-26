#ifndef _IOER_OUTPUT_T_HPP
#define _IOER_OUTPUT_T_HPP
/* module for output utilities in ioer
 *
 * Gaohan
 */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "type_traiter.hpp"
#include "ioer_macros.hpp"
#include "io_base_t.hpp"
#include "ioer_var.hpp"
#include "ioer_exceptions.hpp"

namespace ioer 
{
	using namespace std;
	using namespace type_traiter;
	using ioer::io_base_obj;

	class _pair_output_functor_t 
	{
		protected:
			string _path; 
			string _dlm;
			size_t _width;
			bool _keyfirst;
			bool _nonewline;
			bool _flush;

		public:
			explicit _pair_output_functor_t(const string& path, const string& dlm, size_t width, bool keyfirst, bool nonewline, bool flush) 
				: _path(path), _dlm(dlm), _width(width), _keyfirst(keyfirst), _nonewline(nonewline), _flush(flush) {  }
			virtual ~_pair_output_functor_t() = default;

			// can move
			_pair_output_functor_t(_pair_output_functor_t&&) = default;
			_pair_output_functor_t& operator=(_pair_output_functor_t&&) = default;

			// no copy 
			_pair_output_functor_t(const _pair_output_functor_t&) = delete;
			_pair_output_functor_t& operator=(const _pair_output_functor_t&) = delete;

			// setter
			void config(const string& path, const string& dlm, size_t width, bool keyfirst, bool nonewline, bool flush) noexcept
			{
				_path = path;
				_dlm = dlm;
				_width = width;
				_keyfirst = keyfirst;
				_nonewline = nonewline;
				_flush = flush;
			}

			// operator()
			template<typename KeyType, typename ValType>
				typename enable_if< is_direct_outputable< ValType >::value, const _pair_output_functor_t&>::type
				operator()(const KeyType& key, const ValType& val) const
				{
					iostream& dest = io_base_obj.at(_path);
					if (_keyfirst) 
						dest << setw(_width) << key << _dlm << setw(_width) << val;
					else 
						dest <<  setw(_width) << val << _dlm << setw(_width) << key;
					if (!_nonewline) dest <<  "\n";
					if (_flush) dest << flush;
					return *this;
				}

			template<typename KeyType, typename ValType>
				typename enable_if< is_sequence_container<ValType>::value, const _pair_output_functor_t& >::type
				operator()(const KeyType& key, const ValType& val) const
				{
					iostream& dest = io_base_obj.at(_path);
					if(_keyfirst) {
						dest <<  setw(_width) << key << _dlm;
						for(auto& it : val) dest <<  setw(_width) << it;
					}
					else{
						for(auto& it : val) dest <<  setw(_width) << it;
						dest <<  _dlm << setw(_width) << key ;
					}
					if(!_nonewline) dest <<  "\n";
					if (_flush) dest << flush;
					return *this;
				}
	}; // class _pair_output_functor_t

	class output_t 
	{
		protected:
			string _path;

			size_t _width = 16;
			string _dlm = " ";
			bool _flush = false;

			// functors
			_pair_output_functor_t _pair_output_functor_obj;

		public:
			explicit output_t(const string& path = STDIO_PATH, ios::openmode mode = ios::out)
				: 	_path(path), _pair_output_functor_obj(STDIO_PATH, _dlm, _width, true, false, false)
			{ 
				open(path, mode | ios::out);
			}

			virtual ~output_t() = default;

			// can move
			output_t(output_t&&) = default;
			output_t& operator=(output_t&&) = default;
			
			// no copy 
			output_t(const output_t&) = delete;
			output_t& operator=(const output_t&) = delete;
			
			// getter
			size_t width(void) const noexcept
			{
				return _width; 
			}

			string dlm(void) const noexcept
			{
				return _dlm; 
			}

			bool is_flush(void) const noexcept
			{
				return _flush; 
			}

			// setter
			void set_width(size_t width) noexcept
			{
				_width = width; 
			}

			void set_dlm(const string& dlm) noexcept
			{
				_dlm = dlm; 
			}

			void set_flush(bool flush) noexcept
			{
				_flush = flush; 
			}

			// open/close
			void open(const string& path, ios::openmode mode = ios::out) 
			{
				io_base_obj.open(path, mode);
				_path = path;
			}

			void close(void) noexcept
			{
				io_base_obj.close(_path);
				_path = ioer::STDIO_PATH;
			}

			// utilities

			// -- basic -- //
			void newline(void)
			{
				io_base_obj.at(_path) << "\n"; 
				if (_flush) io_base_obj.at(_path) << flush; 
			}

			void drawline(char c, size_t len = 32) 
			{
				io_base_obj.at(_path) << string(len, c) << "\n"; 
				if (_flush) io_base_obj.at(_path) << flush; 
			}

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
					if (_flush) io_base_obj.at(_path) << flush; 
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
					if (_flush) io_base_obj.at(_path) << flush; 
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
					_pair_output_functor_obj.config(_path, _dlm, _width, _keyfirst, _nonewline, _flush); 
					return _pair_output_functor_obj;
				}

			// -- binary output -- //
			template<typename ParamType>
				typename enable_if<is_fundamental<ParamType>::value, void>::type
				_write(const ParamType& x)
				{
					io_base_obj.at(_path).write
						(reinterpret_cast<const char*>(&x), static_cast<std::streamsize>(sizeof(x)));
				}

			template<typename ParamType>
				typename enable_if<is_complex<ParamType>::value, void>::type
				_write(const ParamType& x)
				{
					using ValType = typename ParamType::value_type;
					ValType tmp[] = {x.real(), x.imag()};
					io_base_obj.at(_path).write
						(reinterpret_cast<const char*>(tmp), static_cast<std::streamsize>(2 * sizeof(ValType)));
				}

			template<typename ParamType>
				typename enable_if<is_c_string<ParamType>::value, void>::type
				_write(const ParamType& x)
				{
					size_t N = 0;
					while(x[N] != '\0') ++N;
					io_base_obj.at(_path).write
						(reinterpret_cast<const char*>(&x), static_cast<std::streamsize>(N * sizeof(char)));
				}

			template<typename ParamType>
				typename enable_if<	is_array_container<ParamType>::value ||
				is_vector<ParamType>::value ||
				is_string<ParamType>::value,
				void>::type
					_write(const ParamType& x)
					{
						io_base_obj.at(_path).write
							(reinterpret_cast<const char*>(x.data()), static_cast<std::streamsize>(x.size() * sizeof(typename ParamType::value_type)));
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
							(reinterpret_cast<const char*>(&x), static_cast<std::streamsize>(sizeof(typename ParamType::value_type)));
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

	// -- non-class functions for standard output -- //
	extern output_t STDOUT;

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

#endif // _IOER_OUTPUT_T_HPP
