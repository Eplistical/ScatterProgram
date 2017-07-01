#ifndef _IOER_T_HPP
#define _IOER_T_HPP
// module for ioer_t

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "ioer_base_t.hpp"
#include "ioer_functors.hpp"

namespace ioer{
	// I/O manager class
	class ioer_t : public ioer_base_t {
		protected:
			// format controller
			size_t _width = 16;
		public:
			// constructor
			explicit ioer_t() = default;
			// cannot copy or assign
			ioer_t(const ioer_t& other) = delete;
			ioer_t& operator=(const ioer_t& other) = delete;
			// destructor
			~ioer_t() = default;
			// read/write binary data
			void read(const std::string& path, char* offset, size_t len);
			void write(const std::string& path, char* offset, size_t len);
			// format setter
			void set_width(size_t w) noexcept { _width = w; }
			// formatted output text data
			void newline(const std::string& path){
				at(path) << "\n";
			}
			void drawline(const std::string& path, char c, size_t len = 32){
				at(path) << std::string(len, c) << "\n";
			}
			template <typename T>
				inline void info(const std::string& path, T msg, bool noendl = false){
					at(path) << msg;
					if(not noendl) newline(path);
				}
			template <typename T>
				inline void info(const std::string& path, const std::vector<T>& s, const std::string& dlm = "\n"){
					for(size_t j = 0, N = s.size(); j < N; j++)
						at(path) << s[j] << dlm;
				}
			keyval_functor keyval(const std::string& path, const std::string& dlm = " : ", const bool keyfirst = true){
				return keyval_functor(static_cast<std::ostream&>(at(path)), dlm, _width, keyfirst);
			}
	};
};
#endif
