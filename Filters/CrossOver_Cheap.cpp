//  Created by Adam Pultz Melbye on 26.12.20.


#include "CrossOver_Cheap.h"

CrossOver_Cheap::CrossOver_Cheap(float frequency){
    init(frequency = 0.1);
}

void CrossOver_Cheap::init(float frequency){}
    
void CrossOver_Cheap::process(float value, float frequency){  // TODO: return array of high and low values
    float x_ = value;
    a_ = frequency;
    b_1 = 1.0f - (2.0f * frequency);
    y_l = ((a_ * x_) + (a_ * x_n_m1) + (b_1 * y_n_m1)); 
    y_h = x_ - y_l;
    y_n_m1 = y_l;
    x_n_m1 = x_; 
    }

float CrossOver_Cheap::getLow(){
    return y_l;
}

float CrossOver_Cheap::getHigh(){
    return y_h;
}


