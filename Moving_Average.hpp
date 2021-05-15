//
//  Moving_Average.hpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 11.03.21.
//

#ifndef Moving_Average_hpp
#define Moving_Average_hpp

#include "CircularBuffer.h"

namespace pultzLib {

class Moving_Average {
public:
    
    Moving_Average(){};
    
    Moving_Average(int size);
    
    void initialise(int size);
    
    float process(float s);
    
    ~Moving_Average() {}
    
private:
    
    CircularBuffer<float> buffer;
    
    int size_;
    double sizeReciprocal_;
    float sum;
    
};

}

#endif /* Moving_Average_h */
