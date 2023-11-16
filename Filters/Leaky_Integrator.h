

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
    
    void reset();
    
    ~Leaky_Integrator() {}
  
private:
    float coeff_ = 0.99;
    float y_m1;
};

#endif /* Leaky_Integrator_h */
