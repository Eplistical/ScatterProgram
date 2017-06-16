#include "ioer_exceptions.hpp"
#include "ioer_base_t.hpp"
#include "ioer_t.hpp"
#include <iostream>
using namespace ioer;

int main(void){
	using namespace std;
	ioer_base_t io;
	io.at("__stdio__") << "hjahalsdfjadsl";
	io.open("1.csa", ios::out);
	io.at("1.csa") << "askdlfjasldkfj";
	io.open("12.csa", ios::out);
	io.at("12.csa") << "askdlfjasldkfj";
	return 0;
}
