//
//  Operators.hpp
//  Genetic Algorithm
//
//  Created by Adam Pultz Melbye on 11.02.21.
//

#ifndef Operators_h
#define Operators_h

# include <libraries/math_neon/math_neon.h>

using namespace std;

class AmpDiff{
    
public:
    
    AmpDiff(int size){
    	size_ = size;
    	doubleSize_ = size_ * 2;
    	inverseSize_ = 1 / size_;
        diffs_.resize(doubleSize_);
        amps_.resize(size_);
        tempMeanAmpDiff_.resize(size_);
    }
    
    void push(float val, int index){
        amps_[index] = val;
        ampSum_ += val;
    }
    
    //  void mean(){
    //  	for ()
    //     amps_[index] = val;
    // }
    
  void process(){
  	
  	meanAmp_ = ampSum_ * inverseSize_;
  	
  	for (unsigned int n = 0; n < size_; n++){
            tempMeanAmpDiff_[n] = fabsf_neon(amps_[n] - meanAmp_);
        };
  	
  	float tempAmpDiff_0 = fabsf_neon(amps_[0] - amps_[3]);
  	float tempAmpDiff_1 = fabsf_neon(amps_[1] - amps_[0]);
  	float tempAmpDiff_2 = fabsf_neon(amps_[2] - amps_[1]);
  	float tempAmpDiff_3 = fabsf_neon(amps_[3] - amps_[2]);
  	
        diffs_[0] += tempAmpDiff_0 + tempMeanAmpDiff_[0];
        diffs_[1] += tempAmpDiff_1 + tempMeanAmpDiff_[0];
        diffs_[2] += tempAmpDiff_1 + tempMeanAmpDiff_[1];
        diffs_[3] += tempAmpDiff_2 + tempMeanAmpDiff_[1];
        diffs_[4] += tempAmpDiff_2 + tempMeanAmpDiff_[2];
        diffs_[5] += tempAmpDiff_3 + tempMeanAmpDiff_[2];
        diffs_[6] += tempAmpDiff_3 + tempMeanAmpDiff_[3];
        diffs_[7] += tempAmpDiff_1 + tempMeanAmpDiff_[3];
    }
    
    vector<float>& getDiffs(){
        return diffs_;
    }
   
    vector<float>& mean(){
        for (unsigned int n = 0; n < doubleSize_; n++){
            diffs_[n] *= inverseSize_;
        };
        
        return diffs_;
    }

    float getMeanAmp(){
        return meanAmp_;
    }
    
    void reset(){
        fill_n(diffs_.begin(), doubleSize_, 0.0); 
    }
    
    ~AmpDiff() {}
    
private:

	int size_;
    int doubleSize_;
    float inverseSize_;
    float ampSum_;
    float meanAmp_;
    vector<float> diffs_;
    vector<float> amps_;
    vector<float> tempMeanAmpDiff_;
    
};

#endif /* Operators_h */
