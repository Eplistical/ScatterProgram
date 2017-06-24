#ifndef _JSON_TOOLKIT_HPP
#define _JSON_TOOLKIT_HPP

#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <sstream>
#include <fstream>
#include "rapidjson/document.h"
#include "boost/lexical_cast.hpp"
#include "boost/bimap.hpp"
using namespace rapidjson;


namespace json{
	// -- convert a json file to rapidjson::Document object -- //
	inline Document load_json_file(const std::string& fname){
		// load string from file
		std::stringstream buf;
		std::ifstream fp(fname);
		buf << fp.rdbuf();
		fp.close();
		// convert to rapidjson::Document
		Document doc;
		doc.Parse(buf.str().c_str());
		return doc;
	}


	// -- functor used to load parameters -- //
	class load_param{
		private:
			const rapidjson::Value& Obj;	

			std::string lower(const std::string& s) const{
				std::string rst = s;
				for(size_t i = 0, N = s.length(); i < N; ++i)
					rst[i] = tolower(s[i]);
				return rst;
			}

		public:
			explicit load_param(const rapidjson::Document& doc, const std::string& category) 
				: Obj(doc[category.c_str()]) 
			{ 
#ifdef _DEBUG
				cout << "load_param: the category now is " << category << "\n"; 
#endif
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const ParamType& param) const{
				// load paramter, scalar version
#ifdef _DEBUG
				cout << "    loading " << key << "\n"; 
#endif
				const char* tag = lower(key).c_str();
				const std::string tmpstr = Obj[tag]["value"][0].GetString();
				const_cast<ParamType&>(param) = boost::lexical_cast<ParamType>(tmpstr);
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const ParamType& param, const boost::bimap<std::string, ParamType>& Dict) const{
				// load parameter & convert to enum class according to Dict, scalar version
#ifdef _DEBUG
				cout << "    loading " << key << "\n"; 
#endif
				const char* tag = lower(key).c_str();
				const std::string tmpstr = Obj[tag]["value"][0].GetString();
				const_cast<ParamType&>(param) = Dict.left.at(tmpstr);
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const std::vector<ParamType>& param) const{
				// load paramter, vector version
#ifdef _DEBUG
				cout << "    loading " << key << "\n"; 
#endif
				const char* tag = lower(key).c_str();
				const auto& data = Obj[tag]["value"];
				std::string tmpstr;
				std::vector<ParamType> rst;
				for(size_t j = 0, N = data.Size(); j < N; ++j){
					tmpstr = data[j].GetString();
					rst.push_back(boost::lexical_cast<ParamType>(tmpstr));
				}
				const_cast<std::vector<ParamType>&>(param) = rst;
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const std::vector<ParamType>& param, const boost::bimap<std::string, ParamType>& Dict) const{
				// load parameter & convert to enum class according to Dict, vector version
#ifdef _DEBUG
				cout << "    loading " << key << "\n"; 
#endif
				const char* tag = lower(key).c_str();
				const auto& data = Obj[tag]["value"];
				std::string tmpstr;
				std::vector<ParamType> rst;
				for(size_t j = 0, N = data.Size(); j < N; ++j){
					tmpstr = data[j].GetString();
					rst.push_back(Dict.left.at(tmpstr));
				}
				const_cast<std::vector<ParamType>&>(param) = rst;
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const std::vector<std::vector<ParamType>>& param) const{
				// load paramter, multiple-vector version
#ifdef _DEBUG
				cout << "    loading " << key << "\n"; 
#endif
				const char* tag = lower(key).c_str();
				const auto& data = Obj[tag]["value"];
				std::string tmpstr;
				std::vector<std::vector<ParamType>> rst;
				for(size_t ivec = 0, Nvec = data.Size(); ivec < Nvec; ++ivec){
					rst.push_back(std::vector<ParamType>());
					for (size_t j = 0, N = data[ivec].Size(); j < N; ++j){
						tmpstr = data[ivec][j].GetString();
						rst[ivec].push_back(boost::lexical_cast<ParamType>(tmpstr));
					}
				}
				const_cast<std::vector<std::vector<ParamType>>&>(param) = rst;
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const std::vector<std::vector<ParamType>>& param, const boost::bimap<std::string, ParamType>& Dict) const{
				// load parameter & convert to enum class according to Dict, multiple-vector version
#ifdef _DEBUG
				cout << "    loading " << key << "\n"; 
#endif
				const char* tag = lower(key).c_str();
				const auto& data = Obj[tag]["value"];
				std::string tmpstr;
				std::vector<std::vector<ParamType>> rst;
				for(size_t ivec = 0, Nvec = data.Size(); ivec < Nvec; ++ivec){
					rst.push_back(std::vector<ParamType>());
					for (size_t j = 0, N = data[ivec].Size(); j < N; j++){
						tmpstr = data[ivec][j].GetString();
						rst[ivec].push_back(Dict.left.at(tmpstr));
					}
				}
				const_cast<std::vector<std::vector<ParamType>>&>(param) = rst;
				return *this;
			}
	};
};

#endif
