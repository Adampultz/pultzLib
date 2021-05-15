//
//  Utilities.hpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 09.03.21.
//

#ifndef Utilities_hpp
#define Utilities_hpp

//#include <base.hpp>
//#include <literals.hpp>
//#include <decibel.hpp>
#include "db_lookup_table.hpp"
#include "fundamentals.hpp"

using namespace pultzLib;
using namespace std;

class level_crossfade
{
public:
    level_crossfade(){};
    
    void initialise(float thresh){
        thresh_ = thresh;
    };
    
    float process(float a, float b, float ctrl){
        if (ctrl < thresh_)
              {
                 auto xfade = float(ctrl - thresh_);
                  xfade = db2a(xfade);
                 return xfade * a + (1.0 - xfade) * b;
              }
              return a;
    }
    
private:
    float thresh_;
};

template <class T>
class Zero_Crossing
{
public:
    Zero_Crossing(){};
    
    int process(T val){
        int sign_ = sgn(val * x_m1);
            if (sign_ == -1){
            x_m1 = val;
            return 1;
            } else {
                if (val == 0){
                    x_m1 = val;
                    return 1;
                } else {
                    x_m1 = val;
                    return 0;
                }
        }
    }
    
private:
    T x_m1;
};


#endif /* Utilities_hpp */

