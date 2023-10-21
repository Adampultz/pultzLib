#pragma once

#ifndef Noise_Gate_hpp
#define Noise_Gate_hpp

#include <stdio.h>
#include "Moving_Average.hpp"
#include "db_lookup_table.hpp"
#include "Utilities.hpp"


#endif /* Noise_Gate_hpp */

namespace pultzLib{

    class Noise_Gate{

    public:
        Noise_Gate();
        
        void init(float threshold, int filterSize);
        
        float process(float source, float db);
        
        ~Noise_Gate() {}
      
    private:
        level_crossfade xFade;
        Moving_Average movAverage;
    };
}
