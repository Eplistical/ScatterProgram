#include <iostream>
#include "randomer.hpp"
using namespace std;

int main(void){
	std::vector<double> rnd = randomer::vrand(20000);
	for(size_t j = 0, N = 200; j < N; ++j){
		cout << rnd.at(j) << endl;
	}

	return 0;
}
