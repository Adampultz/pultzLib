/***** Saturator.h *****/

#ifndef Saturator_h
#define Saturator_h

#include <stdio.h>
#include <cmath>
#include "Leaky_Integrator.h"
#include <libraries/math_neon/math_neon.h>
#include "Utilities.hpp"
#include "fundamentals.hpp"

class Saturator{

public:
    Saturator() {}
    Saturator(float coeff);
    
    void init(float coeff);
    
    float process(float val, float coeff, float amp);
    
    ~Saturator() {}
  
private:
	Leaky_Integrator leakyInt;
    float coeff_ = 0.99;
};

#endif /* Saturator_h */
