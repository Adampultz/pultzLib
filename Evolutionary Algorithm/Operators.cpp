#include "Operators.h"

using namespace pultzLib;

AmpDiff::AmpDiff(int size){
    init(size);
}

void AmpDiff::init(int size){
     size_ = size; // Number of audio inputs
     doubleSize_ = size_ * 2;
     inverseSize_ = 1.0 / size_;
     amps_.resize(size_);
     amps_m_.resize(size_);
     diffs_.resize(doubleSize_);
     tempMeanAmpDiff_.resize(size_);
     tempMeanAmpDiffInd_.resize(doubleSize_);
     tempAmpDiffInd_.resize(doubleSize_);
     tempAmpDiff_.resize(size_);
     ampDiffInd_.resize(doubleSize_);
     // Create array of n - 1 using wrapmax (0 - 1 = size - 1)
     for (unsigned int n = 0; n < size_; n++){
         amps_m_[n] = wrapMax(n - 1, size_ - 1);
     }

//    Create arrays of indexes for indexing into ampdifferences and mean amp differences
     for (unsigned int n = 0; n < doubleSize_; n++){
         int ampDiffInd = ((int) ceil((float) n / 2));
         int meanAmpDiff = (int)  (float) n / 2;
         tempAmpDiffInd_[n] = wrapMax(ampDiffInd, size_ - 1);
         ampDiffInd_[n] = meanAmpDiff;
     }
}

// Push Amplitudes and calculate sum
void AmpDiff::push(float val, int index){
    amps_[index] = val;
    ampSum_ += val;
}

// Calculate amplitude difference
void AmpDiff::process(){
  meanAmp_ = ampSum_ * inverseSize_; // Calculate mean amplitude across all strings
  
  for (unsigned int n = 0; n < size_; n++){
        // Difference between individual string and mean of all strings
        tempMeanAmpDiff_[n] = fabsf_fast(amps_[n] - meanAmp_);
        // Difference between individual strings
        tempAmpDiff_[n] = fabsf_fast(amps_[n] - amps_[amps_m_[n]]);
    };
    
//     Sum differences and add to array (no normalisation happens here, as that will be taken care of in AmpDiff::mean() while calculating the entire generation
  for (unsigned int n = 0; n < doubleSize_; n++){
      diffs_[n] += tempAmpDiff_[ampDiffInd_[n]] + tempMeanAmpDiff_[tempAmpDiffInd_[n]]; // 3
    };
    
    ampSum_ = 0.0f;
}

vector<float>& AmpDiff::getDiffs(){
    return diffs_;
}

//Calculate mean of entire generation
vector<float>& AmpDiff::mean(){
    for (unsigned int n = 0; n < doubleSize_; n++){
        diffs_[n] *= inverseSize_;
    };    
    return diffs_;
}

float AmpDiff::getMeanAmp(){
    return meanAmp_;
}

// Reset amplitude differences to zero
void AmpDiff::reset(){
    fill_n(diffs_.begin(), doubleSize_, 0.0);
}



