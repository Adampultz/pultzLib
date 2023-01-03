/***** Saturator.h *****/

#ifndef Saturator_h
#define Saturator_h

#include <stdio.h>
#include <cmath>
#include "Leaky_Integrator.h"
#include "Fast_Math.hpp"
#include "Utilities.hpp"
#include "fundamentals.hpp"

namespace pultzLib {

class Saturator{

public:
    Saturator() {}
    Saturator(float coeff, float bal);
    
    void init(float coeff, float bal);
    
    void setCoeff(float coeff);
    
    float process(float val, float coeff, float amp);
    
    ~Saturator() {}
  
private:
	Leaky_Integrator leakyInt;
    XFade2 balance;
    
    float coeff_ = 0.99;
    float bal_ = 1.0;
    float amp_ = 1.0;
};

}

#endif /* Saturator_h */
