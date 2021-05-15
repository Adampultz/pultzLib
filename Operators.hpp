//
//  Operators.hpp
//  Genetic Algorithm
//
//  Created by Adam Pultz Melbye on 11.02.21.
//

#ifndef Operators_h
#define Operators_h

using namespace std;

class AmpDiff{
    
public:
    
    AmpDiff(){
        diffs_.resize(8);
        amps_.resize(4);
    }
    
    void push(float val, int index){
        amps_[index] = val;
    }
    
  void process(){
        diffs_[0] += fabs(amps_[0] - amps_[3]);
        diffs_[1] += fabs(amps_[0] - amps_[1]);
        diffs_[2] +=  fabs(amps_[1] - amps_[2]);
        diffs_[3] = diffs_[1];
        diffs_[4] +=  fabs(amps_[2] - amps_[3]);
        diffs_[5] = diffs_[2];
        diffs_[6] =  diffs_[0];
        diffs_[7] =  diffs_[4];
    }
    
    vector<float>& getDiffs(){
      
        return diffs_;
    }
    
    
    vector<float>& mean(int size){
        double inverseSize = 1.0 / size;
        
        for (unsigned int n = 0; n < 8; n++){
            diffs_[n] *= inverseSize;
        };
        
        return diffs_;
    }
    
    void reset(){
        fill_n(diffs_.begin(), 8, 0.0); 
    }
    
    ~AmpDiff() {}
    
private:
    
    vector<float> diffs_;
    vector<float> amps_;
    
};

#endif /* Operators_h */
