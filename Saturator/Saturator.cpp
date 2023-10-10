/***** Saturator.cpp *****/

#include "Saturator.h"

using namespace pultzLib;

Saturator::Saturator(float coeff, float bal, float sensitivity){
    init(coeff, bal, sensitivity);
}

void Saturator::init(float coeff, float bal, float sensitivity){
    coeff_ = coeff;
    bal_ = bal;
    sensitivity_ = sensitivity;
    leakyInt.init(coeff_);
}

void Saturator::setCoeff(float coeff){
    leakyInt.setCoeff(coeff);
}

void Saturator::setSens(float sensitivity){
    sensitivity_ = sensitivity;
}

float Saturator::process(float val, float coeff, float amp){
    leakyInt.setCoeff(coeff);
    float x_ = fabs(val) * sensitivity_;
    float filter = leakyInt.process(x_);
    filter = val * (1.0f / filter);
    float output = Balance2(filter, val, amp);
    
    return output;
}
