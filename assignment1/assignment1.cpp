//****************************************************
// File:   assignment1.cpp
// Author: Daniel Benedi Garcia
// Date:   02/10/2022
// Coms:   Implements both experiments for realizing
//         the experiments of assignment 1.
//****************************************************

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <cmath>

void experiment(uint64_t N, 
                double* rand(), 
                bool eval(double* data, 
                          std::ofstream& out),
                void header(std::ofstream& out)){
    std::ofstream evolution("evolution.csv");
    std::ofstream points("points.csv");

    uint64_t C = 0;
    header(points);
    for(uint64_t n = 1; n <= N; n++){
        double* data = rand();
        if(eval(data, points)){
            C++;
        }
        evolution << ((double)C)/((double)n);
        evolution << std::endl;
    }
}

bool eval_darts(double* data, std::ofstream& out){
    double x = data[0], y = data[1];
    delete[] data;
    out << x << "," << y << std::endl;
    return 1 > (x*x + y*y);
}

double* rand_darts(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0,
                                                1.0);
    
    double* data = new double[2];

    data[0] = dis(gen);
    data[1] = dis(gen);    

    return data;
}

void header_darts(std::ofstream& out){
    out << "X,Y" << std::endl;
}

bool eval_needls(double* data, std::ofstream& out){
    double x = data[0], theta = data[1];
    delete[] data;
    out << x << "," << theta << std::endl;

    return x <= 0.25 * sin(theta);
}

double* rand_needls(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> disX(0.0, 
                                                 0.5);
    static std::uniform_real_distribution<> disT(0, 
                                            M_PI/2.0);

    double* data = new double[2];

    data[0] = disX(gen);
    data[1] = disT(gen);

    return data;
}

void header_needls(std::ofstream& out){
    out << "X,Theta"  << std::endl;
}

int main(int argc, char* argv[]){
    int type = std::atoi(argv[1]);
    if(argc != 2 || ( type != 1 && type != 2 )){
        std::cout << "Usage: " << argv[0] << "type";
        std::cout << std::endl;
        std::cout << "type: Experiment to realize. ";
        std::cout << "1 for darts, ";
        std::cout << "2 for buffon's needles.";
        std::cout << std::endl;
    }
    
    uint64_t N;
    std::cout << "Insert number of runs: ";
    std::cout << std::flush;
    std::cin >> N;
    
    if(type == 1)
        experiment(N, rand_darts, 
                   eval_darts, header_darts);
    else if(type == 2)
        experiment(N, rand_needls,
                   eval_needls, header_needls);
    return 0;
}

