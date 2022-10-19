//
//  Chromosome.hpp
//  Genetic Algorithm
//
//  Created by Adam Pultz Melbye on 07.02.21.
//

#ifndef Chromosome_hpp
#define Chromosome_hpp

#include <stdio.h>
#include <vector>
#include <cmath>
#include <numeric>
#include "Convert.hpp"
#include <random>
#include <string.h>
#include "fundamentals.hpp"

using namespace std;

class Chromosome{

public:
    Chromosome(int popSize, int chrSize, float minVal, float maxVal);
    
    void init(int popSize, int chrSize, float minVal, float maxVal);
    
    vector<float>& getInitPop();
    
    void select(vector<float> &fitness);
    
    void xOver();
    
    void mutation();
    
    vector<float>& getNewFloatPop();
    
    ~Chromosome() {}
  
private:
    Convert convert;
    bool choices[8];
    vector<string> pop_;
    vector<string> tempPop_;
    vector<string> newPop_;
    vector<float> dec_Vals;
    vector<float> selectInd_;
    vector<float> fitNorm_; // Normalised fitness values
    int popSize_;
    int chrSize_;
    int numAlleles_;
    int wrapMaxMax_;
    float minVal_;
    float maxVal_;
    string bin_[8];
    int mutationProb_;
    float range_;
    float popSizeHalf_;
};

#endif /* Chromosome_hpp */
