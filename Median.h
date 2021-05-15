//
//  Median.h
//  LoopingAudioSampleBufferAdvancedTutorial
//
//  Created by Adam Pultz Melbye on 17.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//

#ifndef Median_h
#define Median_h

#include <stdio.h>
#include <algorithm>

class Median
{
public:
    Median(){}
    Median(float in);
    
    ~Median() {}
    
   float process(float in);
    
private:
    float b_ = 0;
    float c_ = 0;
};

#endif /* Median_h */
