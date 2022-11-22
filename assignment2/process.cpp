//*******************************************************
// File:   plots.cpp
// Author: Daniel Benedí García
// Date:   3rd of November of 2022
// Coms:   
//*******************************************************
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cmath>
#include <sstream>
#include <numeric>

using namespace std;

/**
 * Calculates the variance based on equations (15) and (16)
 * on page 216 of "The Art of Computer Programming, Volume 2",
 * Second Edition. Donald E. Knuth. Addison-Wesley Publishing 
 * Company, 1973.
 *
 * It will return the standard deviation which is the squared
 * root of the variance and the mean.
 */

std::pair<double, double> sd_mean(std::vector<uint64_t> samples){
	double s_prev = 0, s_curr = 0;
	double mean_prev = 0, mean_curr = 0;

	for(int idx = 0; idx < samples.size(); idx++){
		// Save previous state
		mean_prev = mean_curr;
		s_prev = s_curr;
		
		// Update new state
		mean_curr = mean_prev + (samples[idx] - mean_prev)/(idx+1);
		s_curr = s_prev + (samples[idx] - mean_prev) * (samples[idx] - mean_curr);
	}

	return std::make_pair(mean_curr, std::sqrt(s_curr/double(samples.size()-1)));
}

/*
std::pair<double, double> sd_mean(std::vector<uint64_t> samples){
	double N = static_cast<double>(samples.size());
	double mean = 0.0;
	for(uint64_t sample : samples){
		double data = static_cast<double>(sample);
		mean += data / N;
	}
	
	double sd = 0.0;
	for(uint64_t sample : samples){
		double data = static_cast<double>(sample);
		sd += (data - mean)*(data - mean)/(N-1.0);
	}
	sd = std::sqrt(sd);
	
	return std::make_pair(mean, sd);
}
*/

void process_quickselect(){
	std::ifstream f("quickselect_comparisons.csv");
	std::ofstream mean("quickselect_mean.dat");
	std::ofstream sd("quickselect_sd.dat");
	std::ofstream median_mean("quickselect_median_mean.dat");
	std::ofstream median_sd("quickselect_median_sd.dat");

	std::string line{};

	const char delim = ',';

	if(!f.good()){
		std::cerr << "error: file open failed 'quickselect_comparisons'." << std::endl;
		return;
	}
	

	int N, j;
	do{
		std::vector<uint64_t> v{};
		uint64_t tmp_int;
		std::string tmp_string {};
		getline(f,line);

		std::stringstream ss(line);
		ss >> N;
        getline (ss, tmp_string, delim);  /* read delimiter */
		ss >> j;
        getline (ss, tmp_string, delim);  /* read delimiter */
		while(ss >> tmp_int){
			v.push_back(tmp_int);
            getline (ss, tmp_string, delim);  /* read delimiter */
		}

		double alpha = double(j)/double(N);
		auto stats = sd_mean(v);
		
		mean << N << "\t" << alpha << "\t" << stats.first/double(N) << std::endl;
		sd << N << "\t" << alpha << "\t" << stats.second/double(N) << std::endl;

		if(std::abs(alpha - 0.5) < 0.005) {
			median_mean << N << "\t" << stats.first/double(N) << std::endl;
			median_sd << N << "\t" << stats.second/double(N) << std::endl;
		}
	}while(!f.eof());
}

void process_quickselect_sample(){
	std::ifstream f("quickselect_sample_comparisons.csv");
	std::ofstream mean("quickselect_sample_mean.dat");
	std::ofstream sd("quickselect_sample_sd.dat");
	std::ofstream median_mean("quickselect_sample_median_mean.dat");
	std::ofstream median_sd("quickselect_sample_median_sd.dat");

	std::string line{};

	const char delim = ',';

	if(!f.good()){
		std::cerr << "error: file open failed 'quickselect_comparisons'." << std::endl;
		return;
	}
	

	int N, j;
	do{
		std::vector<uint64_t> v{};
		uint64_t tmp_int;
		std::string tmp_string {};
		getline(f,line);

		std::stringstream ss(line);
		ss >> N;
        getline (ss, tmp_string, delim);  /* read delimiter */
		ss >> j;
        getline (ss, tmp_string, delim);  /* read delimiter */
		while(ss >> tmp_int){
			v.push_back(tmp_int);
            getline (ss, tmp_string, delim);  /* read delimiter */
		}

		double alpha = double(j)/double(N);
		auto stats = sd_mean(v);
		
		mean << N << "\t" << alpha << "\t" << stats.first/double(N) << std::endl;
		sd << N << "\t" << alpha << "\t" << stats.second/double(N) << std::endl;

		if(std::abs(alpha - 0.5) < 0.005) {
			median_mean << N << "\t" << stats.first/double(N) << std::endl;
			median_sd << N << "\t" << stats.second/double(N) << std::endl;
		}
	}while(!f.eof());
}


void process_randmedian(){
	std::ifstream f("randmedian_comparisons.csv");
	std::ofstream mean("randmedian_mean.dat");
	std::ofstream sd("randmedian_sd.dat");

	std::string line{};

	const char delim = ',';

	if(!f.good()){
		std::cerr << "error: file open failed 'randmedian_comparisons'." << std::endl;
		return;
	}
	

	int N, err;
	do{
		std::vector<uint64_t> v{};
		int tmp_int;
		std::string tmp_string {};

		getline(f,line);
		std::stringstream ss(line);
		
		ss >> err;
        getline (ss, tmp_string, delim);  /* read delimiter */
		ss >> N;
        getline (ss, tmp_string, delim);  /* read delimiter */

		while(ss >> tmp_int){
			std::string tmp_string {};
			v.push_back(tmp_int);
            getline (ss, tmp_string, delim);  /* read delimiter */
		}

		auto stats = sd_mean(v);
		
		mean << N << "\t" << stats.first/double(N) << std::endl;
		sd << N << "\t" << stats.second/double(N) << std::endl;
	}while(!f.eof());
}

int main(){
	process_quickselect();
	process_quickselect_sample();
	process_randmedian();
}

