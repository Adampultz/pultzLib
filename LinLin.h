

#pragma once


class LinLin
{
public:
    LinLin(){}
    LinLin(float inMin, float inMax, float outMin, float outMax);
    
    void setup(float inMin, float inMax, float outMin, float outMax);
    
    ~LinLin() {}
    
   float process(float value);
    
private:
    float a_;
    float b_;
    float inMin_;
    float inMax_;
    float outMin_;
    float outMax_;
};
