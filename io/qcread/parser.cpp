#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <vector>


namespace {
	std::string get_type_string(const rapidjson::Value& val){
		const std::vector<std::string> TypeNames{ 
			"Null", "False", "True", "Object", "Array", "String", "Number" 
		};
		return TypeNames[val.GetType()];
	}

	rapidjson::Document load_json_file(const std::string& fname){
		/* convert a json file to rapidjson::Document object
		 */
		// load string from file
		std::stringstream buf;
		std::ifstream fp(fname);
		buf << fp.rdbuf();
		fp.close();
		// convert to rapidjson::Document
		rapidjson::Document doc;
		doc.Parse(buf.str().c_str());
		return doc;
	}

	void f(const std::string& fname){
		using namespace std;
		rapidjson::Document&& doc = load_json_file(fname);
		rapidjson::Value& rem = doc["rem"];
		cout << rem["dim"]["value"].Size() << endl;
	}
}



int main(){
	f("./try_input.json");
	return 0;
}
