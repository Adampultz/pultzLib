    #include "Amplitude_Sync.h"

    using namespace pultzLib;

    /*
    size = how many individuals are syncing.
    wtSize = size of the wavetable used for storing the oscillator waveform
    sampleRate = sample rate
    makeUpGain = for adjusting gain per string after syncing
     */
        Amplitude_Sync::Amplitude_Sync(int size, int wtSize, float sampleRate, float makeupGain){
            init(size, wtSize, sampleRate, makeupGain);
        };
        
       void Amplitude_Sync::init(int size, int wtSize, float sampleRate, float makeupGain){
           size_ = size;
           wtSize_ = wtSize;
           makeUpGain_ = makeupGain;
           doubleSize_ = size_ * 2;
           weightSumNormalizeCoeff_ = 1.0 / doubleSize_;
           
           oscillator = new WaveTable_Sync<float>[size];
           
           amplitudes_.resize(size);
           oscValues_.resize(size);
           leftIndex_.resize(size);
           rightIndex_.resize(size);
           weightsLeftIndex_.resize(size);
           weightsRightIndex_.resize(size);
           ampDifferences_.resize(size);
           weights_.resize(doubleSize_);
           
           chromosome.init(doubleSize_, 32, 0.2, 1.2);
           AmpVariance.init(size_);
           ampDiff.init(size_);
           
           weights_ = chromosome.getInitPop();
           
           for (unsigned int n = 0; n < doubleSize_; n++){
               weightSum_ += weights_[n];
           }
           
            weightSum_ = weightSum_ * weightSumNormalizeCoeff_; // Make coefficient a function of doubleSize
            ampBalanceTarget_ = makeUpGain_  - weightSum_;
            weightSum_ = 0;
           
            // Create array of n - 1 using wrapmax (0 - 1 = size - 1)
            for (unsigned int n = 0; n < size_; n++){
                leftIndex_[n] = wrapMax(n - 1, size_ -1);
                rightIndex_[n] = wrapMax(n + 1, size_ -1);
                int indexDouble = n * 2;
                weightsLeftIndex_[n] = indexDouble;
                weightsRightIndex_[n] = indexDouble + 1;
                oscillator[n].setup(0.1, wtSize_, sampleRate);
            }
       }
        
        void Amplitude_Sync::push(float val, int index){
            amplitudes_[index] = val;
        }
        
        float Amplitude_Sync::getOscVal(int index){
            return oscValues_[index];
        }
        
        float Amplitude_Sync::getAmpDiffInd(int index){
            return ampDifferences_[index];
        }

        float Amplitude_Sync::getMeanAmp(){
            return ampDiff.getMeanAmp();
        }
        
       void Amplitude_Sync::setWeights(){
           // Calculate amplitude differences and input to chromosome for calculating weights
           weights_ = chromosome.process(ampDiff.getDiffs());
           // Sum all weights
           for (unsigned int k = 0; k < doubleSize_; k++){
                weightSum_ += weights_[k];
                };
           
           weightSum_ = weightSum_ * weightSumNormalizeCoeff_;
           ampBalanceTarget_ = makeUpGain_  - weightSum_;
           weightSum_ = 0;
           ampDiff.reset(); // Reset amplitude differences for new calculation
           
           for(int i = 0; i < 8; ++i)
               printf( "%f\n", weights_[i]);
        }
        
            void Amplitude_Sync::compute(){
                
                ampDiff.process();
          
                for (unsigned int n = 0; n < size_; n++){
                    float ampDiff = (0.5f * ((amplitudes_[leftIndex_[n]] * weights_[weightsLeftIndex_[n]]) + (amplitudes_[rightIndex_[n]] * weights_[weightsRightIndex_[n]]))) - amplitudes_[n];
                    
                        ampDifferences_[n] = ampDiff;
                        oscillator[n].setFrequency(ampDiff);
                }
                
                if (ampBalance_ > ampBalanceTarget_){
                        ampBalance_ -= 0.00001f;
                    } else if (ampBalance_ < ampBalanceTarget_){
                        ampBalance_ += 0.00001f;
                    };
            
            }
        
        float Amplitude_Sync::process(float sig, float amp, int index){
            ampDiff.push(amp, index);
            this->push(amp, index);
            
            float biOsc = oscillator[index].read(); // Bidirectional oscillator
            biOsc = (biOsc * 0.5f + 0.5f); // Unidirectional Oscillator
            oscValues_[index] = biOsc;
            float out = sig * biOsc * ampBalance_;
            return out;
        }
        
