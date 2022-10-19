//
//  Leaky_Integrator.hpp
//  Genetic Algorithm
//
//  Created by Adam Pultz Melbye on 15.02.21.
//

#ifndef Leaky_Integrator_h
#define Leaky_Integrator_h

#include <stdio.h>

class Leaky_Integrator{

public:
    Leaky_Integrator() {}
    Leaky_Integrator(float coeff);
    
    void init(float coeff);
    
    void setCoeff(float coeff);
    
    float process(float val);
    
    ~Leaky_Integrator() {}
  
private:
    float coeff_ = 0.99;
    float y_m1 = 0.0;
};

#endif /* Leaky_Integrator_h */
