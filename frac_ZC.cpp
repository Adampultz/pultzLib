//
//  frac_ZC.cpp
//  LoopingAudioSampleBufferAdvancedTutorial
//
//  Created by Adam Pultz Melbye on 28.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//

#include "frac_ZC.h"

//frac_ZC::frac_ZC(){}

float frac_ZC::process (int x_m1, float y, float y_m1) {
float slope = y - y_m1;
float b = (slope * x_m1) - y_m1;
float xIntCept = b / slope;

return xIntCept;
}
