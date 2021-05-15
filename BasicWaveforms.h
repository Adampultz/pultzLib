//
//  BasicWaveforms.h
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 10.05.21.
//

#ifndef BasicWaveforms_h
#define BasicWaveforms_h


#include <cmath>
#include "WaveTable.h"

namespace pultzLib {

class Triangle_Wave {
public:
    Triangle_Wave() {}                                                    // Default constructor
    Triangle_Wave(float fs, int size, int sampleRate);
              
    void initialise(float fs, int size, int sampleRate){
        int wtSize = size;
        wTable.setup(fs, size, sampleRate);
        
        float inc = 1.0 / (wtSize * 0.5);
        
        for (int i = 0; i < wtSize; i++){
            float val;
            if (i < wtSize / 2){
                val = inc * (i);
            } else {
                val = 2.0 - (inc * i);
            }
            wTable.write(val);
        }
    };
    
    float process(){
        float val = wTable.read();
        return val;
    };

    
    ~Triangle_Wave() {}              // Destructor

private:
    
    WaveTable<float> wTable;
    
    int size_;
    int bufferLength_; // Adjusting the actual buffer to a power of two for masking
    int wrapMask_;
};

}

#endif /* BasicWaveforms_h */
