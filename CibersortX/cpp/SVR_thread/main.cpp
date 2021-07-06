#include <string>
#include <armadillo>
#include <iostream>
#include <fstream>

#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <unistd.h>

#include "SVR.h"

int main() {
	auto start = std::chrono::steady_clock::now();

	std::vector<std::thread> tt;
	for(int i=0; i<3; i++)
		tt.emplace_back(
				SVR,
				//std::move("data/CIBERSORTx_sigmatrix_Adjusted.txt"), 
				//std::move("data/CIBERSORTx_Mixtures_Adjusted.txt"), 
				std::move("data/sig.txt"),
				std::move("data/mix_tmp.txt"),
				(i+1)*0.25, 
				std::to_string((i+1)*25));

	for(int i=0; i<3; i++)
		if(tt[i].joinable())
			tt[i].join();

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::cout << "result: " << diff.count() << "\n";

	return 0;
}

