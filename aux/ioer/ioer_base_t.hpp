#ifndef _IOER_BASE_HPP
#define _IOER_BASE_HPP
// module for ioer_base

#include <string>
#include <map>
#include <fstream>
#include <iostream>

namespace ioer{
	// path for standard I/O (i.e. screen I/O)
	const std::string stdio_path = "__stdio__";

	// I/O manager class
	class ioer_base_t{
		protected:
			std::map<std::string, std::fstream> _fpdict;

			// -- helpers -- //
			bool path_exist(const std::string& path) const noexcept
			{ return (_fpdict.find(path) != _fpdict.end()); }

			bool is_stdio(const std::string& path) const noexcept
			{ return(path == ioer::stdio_path); }

		public:
			// no copy or assign
			explicit ioer_base_t() = default;
			ioer_base_t(const ioer_base_t& other) = delete;
			ioer_base_t& operator=(const ioer_base_t& other) = delete;
			// destructor
			virtual ~ioer_base_t();
			// open/close a source 
			void open(const std::string& path, std::ios::openmode mode);
			void close(const std::string& path);
			// getter
			size_t size(void) const noexcept;
			// access to iostream object
			std::iostream& at(const std::string& path);
	};
};
#endif
