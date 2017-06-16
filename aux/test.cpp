#include <iostream>
#include "matrixop.hpp"
#include "ioer.hpp"
using namespace std;

int main(void){
	std::vector<double> a{1,2,3,2,5,4,3,4,8};
	std::vector<double> v{4,5,6};
	std::vector<double> eva, evt;
	ioer::info(a);
	ioer::info(v);
	ioer::info(matrixop::dmv(a,v));
	return 0;
}
