//*************************************************
// File:   assignment3.cpp
// Author: Daniel Benedi Garcia
// Date:   8th December 2022
// Coms:   Main file of assignment 3 with functions
//         to do the experiments and debug.
//*************************************************

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include "Set.h"

#define M 100

void debug(){
	std::random_device rd;
	std::mt19937_64 g(rd());
	int n = 7, i = 60;

	// Initialize vector with increasing numbers
	// and then shuffle it
	std::vector<int> elems(n, 0);
	std::iota(elems.begin(), elems.end(), 1);
	std::shuffle(elems.begin(), elems.end(), g);


	std::cout << "Input = [" << elems[0];
	for(int i = 1; i < elems.size();i++)
		std::cout << "," << elems[i];
	std::cout << "]" << std::endl;

	double q = i/100.;
	Set<int> set(q);

	// Insert the numbers
	for(int i = 0; i < elems.size(); i++)
		set.insert(elems[i]);

	int nodes;
	std::cout << "levels = ";
	for(int lvl = 0; nodes=set.nodes_lvl(lvl); lvl++)
		std::cout << nodes << ", ";
	std::cout << std::endl;

	std::cout << "Skiplist" << std::endl;
	std::cout << set << std::endl;

	std::cout << "Total search cost = " << set.total_search_cost() << std::endl;
}

void experiment(){
	std::random_device rd;
	std::mt19937_64 g(rd());

	std::ofstream cost("total_search_cost.csv");
	std::ofstream mem("total_memory_use.csv");

	#pragma omp parallel for collapse(2)
	for(int n = 2000; n <= 20000; n += 100){
		for(int i = 10; i < 100; i += 10){
			double q = i/100.;

			std::stringstream buf1, buf2;
			buf1 << n << "," << q;
			buf2 << n << "," << q;

			std::vector<int> elems(n, 0);
			std::iota(elems.begin(), elems.end(), 1);
			for(int m = 0; m < M; m++){
				Set<int> set(q);

				std::shuffle(elems.begin(), elems.end(), g);

				for(int i = 0; i < elems.size(); i++)
					set.insert(elems[i]);

				buf1 << "," << set.total_search_cost();
				buf2 << "," << set.number_pointers();
			}

			buf1 << std::endl;
			buf2 << std::endl;

			#pragma omp critical
			cost << buf1.str();

			#pragma omp critical
			mem << buf2.str();
		}
	}
}

int main(int argc, char* argv[]){
#ifdef DEBUG
	debug();
#else
	experiment();
#endif
}

