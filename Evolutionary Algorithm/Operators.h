
#ifndef Operators_h
#define Operators_h

#include <Fast_Math.hpp>
#include "fundamentals.hpp"


namespace pultzLib{

class AmpDiff{
    
public:
    
    AmpDiff(){}
    AmpDiff(int size);
    
    void init(int size);
    
    void push(float val, int index);
    
    void process();
    
    std::vector<float>& getDiffs();
   
    std::vector<float>& mean();

    float getMeanAmp();
    
    void reset();
    
    ~AmpDiff() {}
    
private:

    int size_;
    int doubleSize_;
    float inverseSize_;
    float ampSum_;
    float meanAmp_;
    std::vector<int> amps_m_;
    std::vector<int> tempMeanAmpDiffInd_;
    std::vector<int> tempAmpDiffInd_;
    std::vector<int> ampDiffInd_;
    std::vector<float> diffs_;
    std::vector<float> amps_;
    std::vector<float> tempMeanAmpDiff_;
    std::vector<float> tempAmpDiff_;
    
};

}

#endif /* Operators_h */

