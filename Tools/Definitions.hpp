//Header file for giving access to definitions

#ifndef Definitions_hpp
#define Definitions_hpp

//Uncomment the below line if you are using the Bela rather a laptop
//#define Bela ;

class Definitions {
public:
    
    Definitions(){};
    
//    static double g_SampleRate;
//    static double g_SampleDur;
//    static double g_Nyquist;
//    static int g_numInputs;
//    static int g_numOutputs;
    
    
//    Definitions(double sampleRate, int numInputs, int numOutputs);
    
    static void initialise(double sampleRate, int numInputs, int numOutputs){
//        g_SampleRate = sampleRate;
//        g_SampleDur = 1.0 / sampleRate;
//        g_Nyquist = sampleRate / 2.0;
//        g_numInputs = numInputs;
//        g_numOutputs = numInputs;
    };
    
    
};

namespace pultzLib {
    static double g_SampleRate = 48000;
    static double g_SampleDur = 1.0 / g_SampleRate;
    static double      g_Nyquist = g_SampleRate / 2.0;
    static int       g_numInputs = 1;
    static int        g_numOutputs = 1;
}


#endif /* Definitions_hpp */
