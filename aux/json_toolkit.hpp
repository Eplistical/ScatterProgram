#ifndef _JSON_TOOLKIT_HPP
#define _JSON_TOOLKIT_HPP

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
				: Obj(doc[category.c_str()]) {}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const ParamType& param) const{
				// load paramter, scalar version
				const char* tag = lower(key).c_str();
				const_cast<ParamType&>(param) = boost::lexical_cast<ParamType>(Obj[tag]["value"][0].GetString());
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const ParamType& param, const boost::bimap<std::string, ParamType>& Dict) const{
				// load parameter & convert to enum class according to Dict, scalar version
				const char* tag = lower(key).c_str();
				const_cast<ParamType&>(param) = Dict.left.at(Obj[tag]["value"][0].GetString());
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const std::vector<ParamType>& param) const{
				// load paramter, vector version
				const char* tag = lower(key).c_str();
				const auto& data = Obj[tag]["value"];
				std::vector<ParamType> rst;
				for(size_t j = 0, N = data.Size(); j < N; ++j){
					rst.push_back(boost::lexical_cast<ParamType>(data[j].GetString()));
				}
				const_cast<std::vector<ParamType>&>(param) = rst;
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const std::vector<ParamType>& param, const boost::bimap<std::string, ParamType>& Dict) const{
				// load parameter & convert to enum class according to Dict, vector version
				const char* tag = lower(key).c_str();
				const auto& data = Obj[tag]["value"];
				std::vector<ParamType> rst;
				for(size_t j = 0, N = data.Size(); j < N; ++j){
					rst.push_back(Dict.left.at(data[j].GetString()));
				}
				const_cast<std::vector<ParamType>&>(param) = rst;
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const std::vector<std::vector<ParamType>>& param) const{
				// load paramter, multiple-vector version
				const char* tag = lower(key).c_str();
				const auto& data = Obj[tag]["value"];
				std::vector<std::vector<ParamType>> rst;
				for(size_t ivec = 0, Nvec = data.Size(); ivec < Nvec; ++ivec){
					rst.push_back(std::vector<ParamType>());
					for (size_t j = 0, N = data[ivec].Size(); j < N; ++j){
						rst[ivec].push_back(boost::lexical_cast<ParamType>(data[ivec][j].GetString()));
					}
				}
				const_cast<std::vector<std::vector<ParamType>>&>(param) = rst;
				return *this;
			}

			template<typename ParamType>
			const load_param& operator()(const std::string& key, const std::vector<std::vector<ParamType>>& param, const boost::bimap<std::string, ParamType>& Dict) const{
				// load parameter & convert to enum class according to Dict, multiple-vector version
				const char* tag = lower(key).c_str();
				const auto& data = Obj[tag]["value"];
				std::vector<std::vector<ParamType>> rst;
				for(size_t ivec = 0, Nvec = data.Size(); ivec < Nvec; ++ivec){
					rst.push_back(std::vector<ParamType>());
					for (size_t j = 0, N = data[ivec].Size(); j < N; j++){
						rst[ivec].push_back(Dict.left.at(data[ivec][j].GetString()));
					}
				}
				const_cast<std::vector<std::vector<ParamType>>&>(param) = rst;
				return *this;
			}
	};
};

#endif
