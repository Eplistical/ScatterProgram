#ifndef _SCATTER_EXCEPTIONS_HPP
#define _SCATTER_EXCEPTIONS_HPP
// module for scatter exceptions

#include <string>
#include <exception>

namespace scatter
{
	// Exceptions
	class ScatterError : std::exception
	{
		protected:
			std::string _title;
			std::string _msg;
		public:
			explicit ScatterError(const std::string& msg = "") :
				_title("ScatterError"), 
				_msg((msg == "")?_title:(_title + ":" + msg)) {}
			// default destructor
			virtual ~ScatterError() = default;
			virtual const char* what() const noexcept{ return _msg.c_str(); }
	};

	class OutofRangeError : public ScatterError
	{
		public:
			explicit OutofRangeError(const std::string& msg = ""){
				_title = "OutofRangeError"; 
				_msg = (msg == "")?_title:(_title + ":" + msg);
			} 
			~OutofRangeError() = default;
	};

	class FileNotFoundError : public ScatterError
	{
		public:
			explicit FileNotFoundError(const std::string& msg = ""){
				_title = "FileNotFoundError"; 
				_msg = (msg == "")?_title:(_title + ":" + msg);
			} 
			~FileNotFoundError() = default;
	};

	class FileNotOpenedError : public ScatterError
	{
		public:
			explicit FileNotOpenedError(const std::string& msg = ""){
				_title = "FileNotOpenedError"; 
				_msg = (msg == "")?_title:(_title + ":" + msg);
			} 
			~FileNotOpenedError() = default;
	};

	class NegativeEigenValueError : public ScatterError
	{
		public:
			explicit NegativeEigenValueError(const std::string& msg = ""){
				_title = "NegativeEigenValueError"; 
				_msg = (msg == "")?_title:(_title + ":" + msg);
			} 
			~NegativeEigenValueError() = default;
	};

	class InvalidModeError : public ScatterError
	{
		public:
			explicit InvalidModeError(const std::string& msg = ""){
				_title = "InvalidModeError"; 
				_msg = (msg == "")?_title:(_title + ":" + msg);
			} 
			~InvalidModeError() = default;
	};
};

#endif
