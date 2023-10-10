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
    Saturator(float coeff, float bal, float sensitivity);
    
    void init(float coeff, float bal, float sensitivity);
    
    void setCoeff(float coeff);
    
    void setSens(float sensitivity);
    
    float process(float val, float coeff, float amp);
    
    ~Saturator() {}
  
private:
	Leaky_Integrator leakyInt;    
    float coeff_ = 0.99;
    float bal_ = 1.0;
    float amp_ = 1.0;
    float sensitivity_ = 8.0;
};

}

#endif /* Saturator_h */
