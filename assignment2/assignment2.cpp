//*******************************************************
// File:   assignment2.cpp
// Author: Daniel Benedí García
// Date: 
// Coms:
//*******************************************************

#include <iostream>
#include <random>
#include <fstream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <sstream>
#include <optional>
#include <omp.h>

/*
 * Recursive implementation
 *	int r = RanPartition(A,p,q); 
	
 *	if(r == j){
 *		return A[r];
 *	}else if( i < r){
 *		return QuickSelect(A,i,min,r-1);
 *	}else{
 *		return QuickSelect(A,i-r,r+1, max);
 *	}
 * 
 *
 * Note: It returns only the number of comparisons, but it can also be returned
 *       the i-th element
 */
int QuickSelect(int* A, int i, int min, int max){
	// Random device and Mersenne twistter are static so they are stored and
	// generated only once
	static std::random_device rd;
	static std::mt19937_64 g(rd());
	
	std::shuffle(A+min, A+max,g);
	
	max--;
	int r;
	int Cj = 0;
	do{
		// Create distribution for range min, max in that iteration
		std::uniform_int_distribution<int> distrib(min,max);
		
		// Choose an element a in A uniformly at random
		int a = A[distrib(g)];
		
		// Order according the pivot element a
		int left = min, right = max;
		while(left < right){
			while(left <= max && A[left]  < a && left < right){ 
				left++;
				Cj++;
			}
			Cj++;
			while(right >= min && A[right] > a && left < right){
				right--;
				Cj++;
			}
			Cj++;
			int temp = A[left];
			A[left] = A[right];
			A[right] = temp;
			left++; right--;
		}

		r = left;
		if(i < r){
			max = r - 1;
		}else if(i > r){
			min = r + 1;
		}
	}while(i != r);

	return Cj;
}

void task1(){
	const int MIN = 20000, MAX = 2000000;
	std::ofstream comparisons("quickselect_comparisons.csv");
	
	#pragma omp parallel for
	for(int N = MIN; N < MAX; N+=1000){
		int* vec = new int[N];
		std::iota(vec, vec+N,1);
		
		std::stringstream buff;
		for(double p = 0.00; p <= 1; p += 0.01){
			int j = int(p*(N-1));
			buff << N << ", " << j;
			for(int attempt = 0; attempt < 100; attempt++){ //Do 100 random tries
				int Cj = QuickSelect(vec, j, 0, N);
				buff << ", " << Cj;
			}
			buff << std::endl;
		}
		comparisons << buff.str();
		delete[] vec;
	}
}

/*
 * Same implementation of QuickSelect but now the pivot will depend on the 
 * sample u.a.r of (2t+1) elements
 */
template<int t>
int QuickSelect_t(int* A, int i, int min, int max){
	// Random device and Mersenne twistter are static so they are stored and
	// generated only once
	static std::random_device rd;
	static std::mt19937_64 g(rd());
	
	std::shuffle(A+min, A+max,g);
	
	max--;
	int r;
	int Cj = 0;
	do{
		// Create distribution for range min, max in that iteration
		std::uniform_int_distribution<int> distrib(min,max);
		
		// Choose 2t+1 elements in A uniformly at random
		int a;
		if( (max+1-min) > (2*t+1) ){
			int S[2*t+1];
			for(int k = 0; k < 2*t+1; k++)
				S[k] = A[distrib(g)];
			std::sort(S,S+2*t+1);
			// Choose pivot depending on the sample
			a = S[(i-min)/((max+1-min)/(2*t+1))];
		}else{
			a = A[distrib(g)];
		}
		
		// Order according the pivot element a
		int left = min, right = max;
		while(left < right){
			while(left <= max && A[left]  < a && left < right){ 
				left++;
				Cj++;
			}
			Cj++;
			while(right >= min && A[right] > a && left < right){
				right--;
				Cj++;
			}
			Cj++;
			int temp = A[left];
			A[left] = A[right];
			A[right] = temp;
			left++; right--;
		}

		r = left;
		if(i < r){
			max = r - 1;
		}else if(i > r){
			min = r + 1;
		}
	}while(i != r);

	return Cj;
}

void task1_extra(){
	const int MIN = 20000, MAX = 2000000;
	std::ofstream comparisons("quickselect_sample_comparisons.csv");
	
	#pragma omp parallel for
	for(int N = MIN; N < MAX; N+=1000){
		int* vec = new int[N];
		std::iota(vec, vec+N,1);
		
		std::stringstream buff;
		for(double p = 0.00; p <= 1; p += 0.01){
			int j = int(p*(N-1));
			buff << N << ", " << j;
			for(int attempt = 0; attempt < 100; attempt++){ //Do 100 random tries
				int Cj = QuickSelect_t<1>(vec, j, 0, N);
				buff << ", " << Cj;
			}
			buff << std::endl;
		}
		comparisons << buff.str();
		delete[] vec;
	}
}



/*
 *	@return The median element of A[min..max]
 *	
 *	R := Sample ceil(n**0.75) from A uniform and w/replacement
 *	Sort(R)
 *	d := floor(0.5*n**0.75-n**0.5) element in R
 *	u := floor(0.5*n**0.75+n**0.5) element in R
 *	
 *	C   := elements between d and u
 *	l_d := num elements lower than d
 *	l_u := num elements bigger than u
 *
 *	if l_d > n/2 or l_u > n/2
 *		FAIL
 *
 *	if |C| > 4n**0.75
 *		FAIL
 *
 *	Sort(C)
 *	return floor(0.5*n) - l_d + 1 element in C
 */
std::optional<std::pair<int,int>> RanSelect(std::vector<int> A){
	static std::random_device rd;
	static std::mt19937_64 g(rd());
	
	std::shuffle(A.begin(), A.end(),g);

	int C_n = 0;
	auto comparator = [&C_n](int a, int b){C_n++; return a < b;};
	
	int n = A.size();
	double n_3_4 = std::pow(n, 0.75);
	double n_1_2 = std::sqrt(n);

	std::vector<int> R(std::ceil(n_3_4));
	
	std::sample(A.begin(), A.end(),
				R.begin(),
				int(std::ceil(n_3_4)), g);

	std::sort(R.begin(), R.end(), comparator);

	int d = R[std::floor(0.5*n_3_4-n_1_2)],
		u = R[std::floor(0.5*n_3_4+n_1_2)];

	std::vector<int> C;
	int l_d = 0, l_u = 0;                                                        
	for(int elem : A){                                                           
	    if(elem < d) l_d++;                                                      
	    else if(elem > u) l_u++;                                                 
	    else C.push_back(elem);                                                  
	    C_n++;                                                                   
	}                                                                            
	                                                                             
	if(l_d > A.size()/2 || l_u > A.size()/2){                                    
		std::cerr << "FAIL: l_d or l_u too big" << std::endl;
		return std::nullopt;                                                     
	}                                                                            
	                                                                             
	if(C.size() > 4*n_3_4){
		std::cerr << "FAIL: C is too big" << std::endl;
	    return std::nullopt;
	}
	                                                                             
	std::sort(C.begin(), C.end(), comparator);                                   
	                                                                             
	return std::optional<std::pair<int,int>>(std::make_pair(C[n/2 - l_d + 1],C_n));
}

void task2(){
	const int MIN = 20000, MAX = 2000000;
	std::ofstream comparisons("randselect_comparisons.csv");
	
	#pragma omp parallel for
	for(int N = MIN; N < MAX; N+=1000){
		std::vector<int> vec(N);
		std::iota(vec.begin(), vec.end(),1);
		
		
		std::stringstream buff;
		int tries = 0;
		for(int attempt = 0; attempt < 100; attempt++){ //Do 100 random tries
			std::optional<std::pair<int,int>> res = std::nullopt;
			while(!res){
				tries++;
				res = RanSelect(vec);
			}
			buff << ", " << res.value().second;
		}
		
		std::stringstream temp;
		temp << N << ", " << tries - 100 << buff.rdbuf() << std::endl;
		buff = std::move(temp);
		comparisons << buff.str();
	}
}
int main(int argc, char* argv[]){
	
	//task1();
	task1_extra();
	//task2();
}

