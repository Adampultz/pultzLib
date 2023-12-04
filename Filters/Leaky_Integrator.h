

#ifndef Leaky_Integrator_h
#define Leaky_Integrator_h

#include <stdio.h>

class Leaky_Integrator{

public:
    Leaky_Integrator() {}
    Leaky_Integrator(double coeff);
    
    void init(double coeff);
    
    void setCoeff(double coeff);
    
    float process(float val);
    
    void reset(float val);
    
    void reset();
    
    ~Leaky_Integrator() {}
  
private:
    double coeff_;
    float y_m1;
};

#endif /* Leaky_Integrator_h */
