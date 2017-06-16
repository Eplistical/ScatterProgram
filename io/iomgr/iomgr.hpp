#ifndef _IO_MGR_HPP
#define _IO_MGR_HPP
// module for io manager

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>

namespace io{
	// path for standard I/O (i.e. screen I/O)
	const std::string stdio_path = "__stdio__";

	// I/O manager class
	class iomgr_t{
		protected:
			std::map<std::string, std::fstream> _fpdict;
			// format controller
			size_t _default_width = 16;
			// helper
			bool path_exist(const std::string& path);
			bool is_stdio(const std::string& path);
		public:
			// constructor
			explicit iomgr_t() = default;
			// destructor
			~iomgr_t();
			// cannot copy or assign
			iomgr_t(const iomgr_t& other) = delete;
			iomgr_t& operator=(const iomgr_t& other) = delete;
			// open/close a source 
			void open(const std::string& path, std::ios::openmode mode);
			void close(const std::string& path);
			// getter
			size_t size(void);
			// access to iostream object
			std::iostream& at(const std::string& path);
			// read/write binary data
			void read(const std::string& path, char* offset, size_t len);
			void write(const std::string& path, char* offset, size_t len);
	};

	// Exceptions
	class IOError : std::exception{
		protected:
			std::string _title;
			std::string _msg;
		public:
			explicit IOError() : 
				_title("IOError"), _msg(_title){}
			explicit IOError(const std::string& msg) :
				_title("IOError"), _msg(_title + ": " + msg){}
			virtual ~IOError(){}
			virtual const char* what() const noexcept{ return _msg.c_str(); }
	};

	class FileNotOpenedError : public IOError{
		public:
			explicit FileNotOpenedError(){
				_title = "FileNotOpenedError"; _msg = _title; 
			} 
			explicit FileNotOpenedError(const std::string& msg){
				_title = "FileNotOpenedError"; _msg = _title + ":" + msg; 
			} 
			~FileNotOpenedError(){}
	};

	class FileNotExistError : public IOError{
		public:
			explicit FileNotExistError(){
				_title = "FileNotExistError"; _msg = _title; 
			} 
			explicit FileNotExistError(const std::string& msg){
				_title = "FileNotExistError"; _msg = _title + ":" + msg; 
			} 
			~FileNotExistError(){}
	};

	class FileModeIncompatibleError : public IOError{
		public:
			explicit FileModeIncompatibleError(){
				_title = "FileModeIncompatibleError"; _msg = _title; 
			} 
			explicit FileModeIncompatibleError(const std::string& msg){
				_title = "FileModeIncompatibleError"; _msg = _title + ":" + msg; 
			} 
			~FileModeIncompatibleError(){}
	};
};
#endif
