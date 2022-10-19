//
//  frac_ZC.hpp
//  LoopingAudioSampleBufferAdvancedTutorial
//
//  Created by Adam Pultz Melbye on 28.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//

#ifndef frac_ZC_hp
#define frac_ZC_hp

#include <stdio.h>

class frac_ZC {
  public:
    
    frac_ZC () {}                                                    // Default constructor
              
    float process (int, float, float);
    
    ~frac_ZC () {}
};
#endif /* frac_ZC_hp */
