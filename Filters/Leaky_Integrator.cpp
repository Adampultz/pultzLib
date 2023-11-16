//
//  Leaky_Integrator.cpp

//  Created by Adam Pultz Melbye on 15.02.21.
//

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

void Leaky_Integrator::reset(){
    y_m1 = 0.0;
}

float Leaky_Integrator::process(float val){
    float y = val + coeff_ * y_m1;
    y_m1 = y;
    
    return y;
    
}
