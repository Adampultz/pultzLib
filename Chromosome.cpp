//
//  Chromosome.cpp
//  Genetic Algorithm
//
//  Created by Adam Pultz Melbye on 07.02.21.
//

#include "Chromosome.hpp"

Chromosome::Chromosome(int popSize, int chrSize, float minVal, float maxVal){
    Chromosome::init(popSize, chrSize, minVal, maxVal);
}

void Chromosome::init(int popSize, int chrSize, float minVal, float maxVal){
    popSize_ = popSize;
    popSizeHalf_ = popSize_ / 2;
    chrSize_ = chrSize;
    minVal_ = minVal;
    maxVal_ = maxVal;
    range_ = maxVal - minVal_;
    wrapMaxMax_ = popSizeHalf_ - 1;
    dec_Vals.resize(popSize_);
    pop_.resize(popSize_, string(chrSize_, 0));
    tempPop_.resize(popSize_, string(chrSize_ - 6, 0));
    newPop_.resize(popSize_, string(chrSize_, 0));
    selectInd_.resize(popSize_ * 0.5f);
    fitNorm_.resize(popSize_);
    numAlleles_ = popSize_ * chrSize_;
    mutationProb_ = numAlleles_ - 7;
    
    srand (static_cast <unsigned> (time(0)));
    
    for(unsigned int n = 0; n < popSize_; n++){
        tempPop_[n] = "";
        float r = uniform_Rand_dec(minVal_, maxVal_);
        dec_Vals[n] = r;
        pop_[n] = convert.GetBinary32(r);
    }

}

vector<float>& Chromosome::getInitPop(){
    return dec_Vals;
}

void Chromosome::select(vector<float> &fitness){
    float fitnessSum_ = 0.0f;
    for (int i = 0; i < popSize_; i++){
        fitnessSum_ += fitness[i];
    }
        
    for(unsigned int n = 0; n < popSize_ * 0.5f; n++){
        float random = uniform_Rand_dec(0.0f, fitnessSum_);
        for(int i = 0; i < popSize_; i++){
            if(choices[i] == false){
                if(random < fitness[i]){
                    selectInd_[n] = i;
                    choices[i] = true;
                    break;
                }else{
                random -= fitness[i];
                }
            }
            random -= fitness[i];
        }
    }    
}

void Chromosome::xOver(){
    for (int n = 0; n < popSizeHalf_; n++){
        int selInd = wrapMax((n + 1), wrapMaxMax_);
        int tempInd = (n * 2) ;
        int tempInd_2 = tempInd + 1;
        int xOverPoint = uniform_Rand_int(0, chrSize_ - 6);
        int r_Size = chrSize_ - xOverPoint;
        int r_Pos = xOverPoint + 6;
        char chr_1_L[26] = "";
        char chr_1_R[26] = "";
        char chr_2_L[26] = "";
        char chr_2_R[26] = "";
        pop_[selectInd_[n]].copy(chr_1_L, xOverPoint, 6);
        pop_[selectInd_[n]].copy(chr_1_R, r_Size, r_Pos);
        pop_[selectInd_[selInd]].copy(chr_2_L, xOverPoint, 6);
        pop_[selectInd_[selInd]].copy(chr_2_R, r_Size, r_Pos);
        string one_l(chr_1_L);
        string one_R(chr_1_R);
        string two_l(chr_2_L);
        string two_R(chr_2_R);
        tempPop_[tempInd] = one_l + two_R;
        tempPop_[tempInd_2] = two_l + one_R;
    }
}

void Chromosome::mutation(){
    for (int n = 0; n < popSize_; n++){
        for (int i = 0; i < chrSize_ - 6; i++){
            int random = uniform_Rand_int(0, mutationProb_);
            if(random < 1){
                if(tempPop_[n][i] == 1){
                    tempPop_[n].replace(i, 1, "0");
                } else {
                    tempPop_[n].replace(i, 1, "1");
                }
            }
        }
    }
    
}

vector<float>& Chromosome::getNewFloatPop(){
    
    for(unsigned int n = 0; n < popSize_; n++){
        choices[n] = false; // Reset choices array
        newPop_[n] = "001111" + tempPop_[n];
        dec_Vals[n] = convert.GetFloat32(newPop_[n]) * range_ + minVal_;
    }
    return dec_Vals;
}
