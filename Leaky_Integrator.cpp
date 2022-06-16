#include "Leaky_Integrator.h"

Leaky_Integrator::Leaky_Integrator(float coeff){
    init(coeff);
}

void Leaky_Integrator::init(float coeff){
	coeff_ = coeff;
}

void Leaky_Integrator::setCoeff(float coeff){
    coeff_ = coeff;
}

float Leaky_Integrator::process(float val){
    float y = val + coeff_ * y_m1;
    y_m1 = y;
    
    return y;
    
}
