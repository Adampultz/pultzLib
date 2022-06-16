/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
	Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
	Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt
*/

// Render function for FAAB

#include <Bela.h>
#include <algorithm>
#include <libraries/Scope/Scope.h>
#include <cmath>
#include <cstdio>
#include <random>
#include <libraries/Biquad/Biquad.h>
#include <libraries/GuiController/GuiController.h>
#include <libraries/math_neon/math_neon.h>
#include "Wavelets_RT_Shell.h"
#include "Spectral_Balance_Cheap.h"
#include "LinLin.h"
#include "RMS.h"
#include "Median.h"
#include "Wavetable.h"
#include "EnvelopeFollower.h"
#include "Wavetable.h"
#include "Saturator.h"
#include "LinLin.h"
#include "Chromosome.hpp"
#include "Operators.hpp"
#include "Moving_Average.hpp"
#include "fundamentals.hpp"
#include "Utilities.hpp"
#include "ZC_Detect.hpp"
#include "DelayLin.h"
#include "OnePole_LP.h"
#include "Leaky_Integrator.h"

using namespace pultzLib;

unsigned int gAudioChannelNum; // number of audio channels to iterate over
unsigned int gAnalogChannelNum; // number of analog channels to iterate over
unsigned int gNumIns = 4; // Number of inputs (could be different than audio channels)
unsigned int gVarUpdateSens = 64; // Frequency (in samples) for updating non-audio control signals
unsigned int gVarUpdateCount;
float lifeSpanSec = 30; // Initial life span
float lifeSpan = 48000 * lifeSpanSec; // Initial life span in seconds
std::vector<float> weights;
std::vector<float> ampDiffMeans;
std::vector<float> ampDiffPoll;
float gSpectralBalance[4];
float out[4];
float in[4];
float gStretch[4];
float gInGain[4] = {0};
float gStringBal[4] = {1.3, 1.3, 1.0, 1.0};
float source[4];
float dc[4];
float gLpFreq = 40;
float gFilterQ = 0.707;
float rmsVal[4];
float rmsValSlow[4];
float slider[4] = {0.0};
float gMaxSlide = 3.3/4.096;
float volume[4] = {0.0};
float gGain[4] = {1.0};
float gEnvelopeOutput[4] = {0.0};
float kOscillatorFrequency[4] = {0.1, 0.12, 0.13, 0.14};
float gAmpDiff[4] = {0.01};
float gOsc[4] = {0.0};
float syncAmpAdjust[4] = {1.3, 1.0, 1.0, 1.0};
float weightSum;
float ampBalance = 1.0;
float ampBalanceTarget = 1.0;
int gGenCounter = 0;
float ampVarianceSum;
float rmsIn[4] = {0.1};
int balance;
bool genCount = false;
float envTest[4];
float _movAvg[4];
float testFade[4];
float dB[4];
float onePole[4];
float rmsDiff[4];
float actualDT[4] = {0.01};
float targetDT[4] = {0.01};
float feedBackSign[4] = {1.0}; //Sign of feedback coefficient for comb filter
int numRatios = 5;
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, numRatios - 1);
float ratios[5] = {1.5, 1.25, 0.875, 1.125, 1.375};
float delTimeSmoothCoeff = 1.0 / (48000 * 0.1);
float delTimeSmoothUp = 1.0 + delTimeSmoothCoeff;
float delTimeSmoothDown = 1.0 - delTimeSmoothCoeff;
float minDbMap[4] = {-45, -45, -50, -50}; // Lower Decibel threshold for individual strings
float minLifeSpan_ = 10.0;
float maxLifeSpan_ = 30;

Wavetable* gOscillator;
EnvelopeFollower* envelope;
Wavelets_RT_Shell* wavelets;
Spectral_Balance_Cheap* specBal;
RMS* rms;
RMS* rmsSlow;
Biquad* highPass;
Saturator* saturator;
Chromosome chromosome(8, 32, 0.2, 1.2);
AmpDiff meanAmps(gNumIns);
EnvelopeFollower* envIn;
Moving_Average* movAvg;
level_crossfade* xFade;
ZC_Detect<float>* zero_crossings;
DelayLin* delay;
OnePole_LP* zcFilt;
OnePole_LP* rmsDiffSmooth;
OnePole_LP* triggerFilt;
Leaky_Integrator* ampDiffInt;
Schmitt_Trigger* schmitt_trigger;
Variance<float> AmpVariance(gNumIns);
Scope scope;

AuxiliaryTask gGA;
void process_GA_background(void *);
	
bool setup(BelaContext *context, void *userData)
{
	
	std::vector<float> wavetable;
	unsigned int wavetableSize = 2048;
	wavetable.resize(wavetableSize);
	for(unsigned int n = 0; n < wavetableSize; n++ ){
    wavetable[n] = sin(2.0 * M_PI * (float)n / (float)wavetableSize);
	};

gOscillator = new Wavetable[gNumIns];
highPass = new Biquad[gNumIns];
envelope = new EnvelopeFollower[gNumIns];
wavelets = new Wavelets_RT_Shell[gNumIns];
envIn = new EnvelopeFollower[4];
movAvg = new Moving_Average[4];
xFade = new level_crossfade[4];
zero_crossings = new ZC_Detect<float>[4];
rms = new RMS[gNumIns];
rmsSlow = new RMS[gNumIns];
delay = new DelayLin[gNumIns];
zcFilt = new OnePole_LP[gNumIns];
rmsDiffSmooth = new OnePole_LP[gNumIns];
specBal = new Spectral_Balance_Cheap[gNumIns];
ampDiffInt = new Leaky_Integrator[gNumIns];
triggerFilt = new OnePole_LP[gNumIns];
schmitt_trigger = new Schmitt_Trigger[gNumIns];

weights = chromosome.getInitPop();
	for (unsigned int k = 0; k < 8; k++){
						 weightSum += weights[k];
					};
					weightSum = weightSum * 0.125f;
					ampBalanceTarget = 1.0f + (1.0f - sqrtf_neon(weightSum)); 
					weightSum = 0.0f;

Biquad::Settings settings{
			.fs = context->audioSampleRate,
			.cutoff = gLpFreq,
			.type = Biquad::highpass,
			.q = gFilterQ,
			.peakGainDb = 0,
			};
		
				
	for (unsigned int n = 0; n < gNumIns; n++) {
	
	wavelets[n].setup(context->audioSampleRate, 0.5, 10, 0.01, 2, 1, 5);
	rms[n].setup(50.0, context->audioSampleRate);
	rmsSlow[n].setup(0.2, context->audioSampleRate);
	highPass[n].setup(settings);
	gOscillator[n].setup(context->audioSampleRate, wavetable);
	gOscillator[n].setFrequency(kOscillatorFrequency[n]);
	envIn[n].setup(0.01, 0.15, context->audioSampleRate);
	movAvg[n].initialise(16);
	xFade[n].initialise(-37.0);
	zero_crossings[n].initialise(30, context->audioSampleRate);
	delay[n].setup(0.8, 1.0 / 96.0, 0.8, context->audioSampleRate);
	zcFilt[n].setFc(1, context->audioSampleRate);
	rmsDiffSmooth[n].setFc(10, context->audioSampleRate);
	specBal[n].initialise(0.01, 15.0, context->audioSampleRate);
	ampDiffInt[n].init(0.9);
	schmitt_trigger[n].init(0.04, 0.015);
	triggerFilt[n].setFc(10, context->audioSampleRate);
	}
	
	gGA = Bela_createAuxiliaryTask(process_GA_background, 3, "bela-process-GA");
	
	if(context->audioInChannels != context->audioOutChannels ||
			context->analogInChannels != context-> analogOutChannels){
		printf("Different number of outputs and inputs available. Working with what we have.\n");
			}
			
	gAudioChannelNum = std::min(context->audioInChannels, context->audioOutChannels);
	gAnalogChannelNum = std::min(context->analogInChannels, context->analogOutChannels);

	return true;
}

void process_GA_background(void *){
	ampVarianceSum /= lifeSpan;
	ampVarianceSum = clamp2(ampVarianceSum, 0.000001f, 0.3f);
	ampDiffMeans = meanAmps.getDiffs();
	chromosome.select(ampDiffMeans);
    chromosome.xOver();
    chromosome.mutation();
    weights = chromosome.getNewFloatPop();

    for (unsigned int k = 0; k < 8; k++){
		weightSum += weights[k];
	};
    
	weightSum = weightSum * 0.125f;
	ampBalanceTarget = 2.5f  - weightSum;
	float newLifeSpan = 40.0 - clamp2(mapLin(ampVarianceSum, 0, 0.3f, minLifeSpan_, maxLifeSpan_), minLifeSpan_, maxLifeSpan_);
	// rt_printf("Life Span: %f \n", newLifeSpan);
	lifeSpan = 48000 * newLifeSpan;
	meanAmps.reset();
	weightSum = 0.0f;
	ampVarianceSum = 0;
    genCount = true;
}

void render(BelaContext *context, void *userData)
{

	for(unsigned int n = 0; n < context->audioFrames; n++) {
		 float mix = 0.0;
		 float stretchMix = 0.0;
		 float balSync = clamp2(mapLin(analogRead(context, n / 2, 4),  0.01f, 3.3 / 4.096, 0.0f, 1.0f), 0.0f, 1.0f);
        
		 if (balSync < 0.01f){ // Prevent generation counter from updating if GA is not used
		 genCount = false;
		 } else {
			genCount = true;
		 };
		 
		 float balStretch = clamp2(mapLin(analogRead(context, n / 2, 5) ,  0.01f, 3.3 / 4.096, 0.0f, 1.0f), 0.0f, 1.0f);
		 float balSat = clamp2(mapLin(analogRead(context, n / 2, 6),  0.01f, 3.3 / 4.096, 0.0f, 1.0f), 0.0f, 1.0f);
		 
		 if (ampBalance > ampBalanceTarget){
		    	ampBalance -= 0.00001f;
		    } else if (ampBalance < ampBalanceTarget){
		    	ampBalance += 0.00001f;
		    };

		for(unsigned int ch = 0; ch < gNumIns; ch++){
			volume[ch] = mapLin(analogRead(context, n / 2, ch),  0.001f, 3.3 / 4.096, 0.0f, 1.0f);
			source[ch] = audioRead(context, n, ch);
			
			envTest[ch] = envIn[ch].process(source[ch]);
			
			_movAvg[ch] = movAvg[ch].process(source[ch]);
			
			dB[ch] = envIn[ch].getdb();
			
			float movavgFilt = xFade[ch].process(source[ch], _movAvg[ch], dB[ch]);

			dc[ch] = highPass[ch].process(movavgFilt);
			
			specBal[ch].process(stretchMix);
		
			gSpectralBalance[ch] = specBal[ch].getTendencyHZ();
			
			onePole[ch] = zcFilt[ch].process(clamp2(gSpectralBalance[ch], 40.0f, 1000.0f));
			
			gEnvelopeOutput[ch] = clamp2(mapLin(dB[ch], minDbMap[ch], -5.0f, 0.0f, 1.0f), 0.0f, 1.0f);

			meanAmps.push(gEnvelopeOutput[ch], ch);
		
			int left = wrapMax(ch - 1, 3);
			int right = wrapMax(ch + 1, 3);
			int ch2 = ch * 2;

			gAmpDiff[ch] = (0.5f * ((gEnvelopeOutput[left] * weights[ch2]) + (gEnvelopeOutput[right] * weights[ch2 + 1]))) - gEnvelopeOutput[ch];

			gOscillator[ch].setFrequency(gAmpDiff[ch]);
			gOsc[ch] = gOscillator[ch].process();
		    gOsc[ch] = (gOsc[ch] * 0.5f + 0.5f);
	
		    
			float out = dc[ch] * gOsc[ch] * ampBalance;
			out = Balance2(dc[ch], out, balSync);
			
			float stretch = wavelets[ch].process(dc[ch]);

			float blend = Balance2(out, stretch, balStretch);
			
			float delay_ = delay[ch].process(blend);
			
			float delayBlend = Balance2(blend, delay_, balSat);

			rmsVal[ch] = rms[ch].process(delayBlend);
			float rmsSlowFilt = rmsSlow[ch].process(delayBlend);
			rmsValSlow[ch] = rmsDiffSmooth[ch].process(fabsf_neon(rmsSlowFilt - rmsVal[ch]));
			rmsValSlow[ch] = ampDiffInt[ch].process(rmsValSlow[ch]);
			
			feedBackSign[ch] = schmitt_trigger[ch].process(rmsVal[ch]);
			feedBackSign[ch] = triggerFilt[ch].process(feedBackSign[ch]);
			
			if (rmsValSlow[ch] > 0.1){
			int ratioChoice = distribution(generator); // Random integer between 0 and numRatios - 1
			float ratio = ratios[ratioChoice]; // Float value of ratio
			targetDT[ch] = 1.0f / (onePole[ch] * ratio); // Delay time (inverse of estimated frequency times ratio)
			}
			
			if (actualDT[ch] < targetDT[ch]){
			actualDT[ch] *= delTimeSmoothUp;
			}
			else if (actualDT[ch] > targetDT[ch]){
			actualDT[ch] *= delTimeSmoothDown;	
			}
			
			actualDT[ch] =  clamp2(actualDT[ch], 0.00004f, 0.03f);
			
			delay[ch].setFeedback(clamp2(float(0.9f - rmsValSlow[ch]), 0.0f, 0.9f ) * (feedBackSign[ch] * -1));
			
			delay[ch].setDelayTime(actualDT[ch]); // = 1.0 / ratio / zc
			
			AmpVariance.push(rmsVal[ch], ch);
			
			float gainAdjust = delayBlend * volume[ch] * gStringBal[ch];
			
			mix += gainAdjust;
			stretchMix += stretch;
			
			audioWrite(context, n, ch + 2, gainAdjust);
		}

		audioWrite(context, n, 1, mix); // Write summed audio to output 1 (going to amp)
		
		gVarUpdateCount++; // Update control signal counter
		
		if(gVarUpdateCount > gVarUpdateSens){ // Counter for updating control signals
			  gVarUpdateCount = 0;
			  for(unsigned int i = 0; i < gNumIns; i++){
                  int	balance_ = (int) mapLin(clamp2(gSpectralBalance[i], 30.0f, 1500.0f), 40.0f, 1000.0f, 8, 1);
			  int rmsLin = mapLin(rmsVal[i], 0.0f,  0.3f, 1, 10);
			  rmsLin = clamp2((int) rmsLin, 1, 10);
			  wavelets[i].setVariables(rmsLin, 1, balance_);
			  wavelets[i].setAnalysisWindow(clamp2(rmsValSlow[i], 0.1f, 0.9f ));
			  }
		}
		
		meanAmps.process();
		ampVarianceSum += AmpVariance.process(meanAmps.getMeanAmp());
				
		if (genCount == true){
			gGenCounter++; // Update generation counter
			if(gGenCounter > lifeSpan - 1){ // If generation counter has reached the end of current life span
				Bela_scheduleAuxiliaryTask(gGA); // Perform auxillary task
				gGenCounter = 0; // Reset generation counter
                genCount = false; // Prevent generation counter from updating while auxillary task is running
			}
			
		}

	}
}

void cleanup(BelaContext *context, void *userData)
{
	
}
