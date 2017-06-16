#ifndef _STRING_MODE_CONVERSION_HPP
#define _STRING_MODE_CONVERSON_HPP
/* module for string & mode conversion */
#include <string>
#include <vector>
#include "boost/bimap.hpp"

namespace enumspace{
	template<typename ParamType>
	ParamType string_to_mode(const std::string& key, const boost::bimap<std::string, ParamType>& Dict){
		return Dict.left.at(key);
	}

	template<typename ParamType>
	std::vector<ParamType> string_to_mode(const std::vector<std::string>& key, const boost::bimap<std::string, ParamType>& Dict){
		std::vector<ParamType> rst;
		for(auto& it : key)
			rst.push_back(Dict.left.at(it));
		return rst;
	}

	template<typename ParamType>
	std::vector<std::vector<ParamType>> string_to_mode(const std::vector<std::vector<std::string>>& key, const boost::bimap<std::string, ParamType>& Dict){
		std::vector<std::vector<ParamType>> rst;
		for(size_t ivec = 0, Nvec = key.size(); ivec < Nvec; ++ivec){
			rst.push_back(std::vector<ParamType>());
			for(size_t j = 0, N = key.at(ivec).size(); j < N; ++j){
				rst.at(ivec).push_back(Dict.left.at(key.at(ivec).at(j)));
			}
		}
		return rst;
	}

	template<typename ParamType>
	std::string mode_to_string(const ParamType& mode, const boost::bimap<std::string, ParamType>& Dict){
		return Dict.right.at(mode);
	}

	template<typename ParamType>
	std::vector<std::string> mode_to_string(const std::vector<ParamType>& mode, const boost::bimap<std::string, ParamType>& Dict){
		std::vector<std::string> rst;
		for(auto& it : mode)
			rst.push_back(Dict.right.at(it));
		return rst;
	}

	template<typename ParamType>
	std::vector<std::vector<std::string>> mode_to_string(const std::vector<std::vector<ParamType>>& mode, const boost::bimap<std::string, ParamType>& Dict){
		std::vector<std::vector<std::string>> rst;
		/*
		for(size_t ivec = 0, Nvec = mode.size(); ivec < Nvec; ++ivec){
			rst.push_back(std::vector<std::string>());
			for(size_t j = 0, N = mode.at(ivec).size(); j < N; ++j){
				rst.at(ivec).push_back(Dict.right.at(mode.at(ivec).at(j)));
			}
		}
		*/
		return rst;
	}
};
#endif
