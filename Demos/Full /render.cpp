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

#define numInputs 4 

#include <Bela.h>
#include <algorithm>
#include <libraries/Scope/Scope.h>
#include <cmath>
#include <cstdio>
#include "Wavesets_RT_Shell.h"
#include "Spectral_Balance_Cheap.h"
#include "RMS.h"
#include <libraries/Biquad/Biquad.h>
#include <libraries/GuiController/GuiController.h>
#include <libraries/math_neon/math_neon.h>
#include "EnvelopeFollower.h"
#include "Chromosome.hpp"
#include "Operators.hpp" // fix operators.h
#include "Moving_Average.hpp"
#include "fundamentals.hpp"
#include "Utilities.hpp"
#include "OnePole_LP.h"
#include "Amplitude_Sync.h"
#include "Adaptive_Comb.h"
#include "Leaky_Integrator.h"
#include "OnePole_LP.h"
#include "DelayLin.h"

using namespace pultzLib;

unsigned int gAudioChannelNum; // number of audio channels to iterate over
unsigned int gAnalogChannelNum; // number of analog channels to iterate over
unsigned int gVarUpdateSens = 64; // Control rate frequency
unsigned int gVarUpdateCount;
float lifeSpanSec = 30; // Lifespan of Evolutionary Algorithm in seconds
float lifeSpan = 48000 * lifeSpanSec; // Lifespan of Evoluionary Algorithm in samples
std::vector<float> weights_; // Create vector of weights for Evolutionary Algorithm
std::vector<float> ampDiffMeans; // Create vector of mean amplitude differences for Evolutionary Algorithm
float gSpectralBalance[numInputs]; // Array of spectral tendency values
float out[numInputs];
float in[numInputs];
float gStretch[numInputs];
float gStringBal[numInputs] = {1.2, 1.1, 1.0, 1.0}; // Amplitude multiplication values for individual strings
float source[numInputs];
float dc[numInputs];
float gLpFreq = 40;
float gFilterQ = 0.707;
float rmsVal[numInputs];
float rmsValSlow[numInputs];
float slider[numInputs] = {0.0};
float volume[numInputs] = {0.0};
float gEnvelopeOutput[numInputs] = {0.0};
float gAmpDiff[numInputs] = {0.01};
float weightSum;
float ampBalance = 1.0;
float ampBalanceTarget = 1.0;
int gGenCounter = 0;
float ampVarianceSum;
float rmsIn[numInputs] = {0.1};
int balance;
bool genCount = false;
float envTest[numInputs];
float _movAvg[numInputs];
float dB[numInputs];
float onePole[numInputs];
float rmsDiff[numInputs];
float minDbMap[numInputs] = {-45, -45, -50, -50};
float minLifeSpan_ = 10.0;
float maxLifeSpan_ = 30;
int wtSize_ = 2048;
float stretch_[numInputs] = {0};
float fbSgn[numInputs] = {1};
float schmittLowThres[numInputs] = {0.015, 0.015, 0.015, 0.015};
float schmittHighThres[numInputs] = {0.13, 0.12, 0.12, 0.12};
float schmittSmoothFreq[numInputs] = {8, 8, 8, 8};
float delayBlend[numInputs] = {0};

EnvelopeFollower* envelope;
Wavesets_RT_Shell* wavesets;
Spectral_Balance_Cheap* specBal;
RMS* rms;
RMS* rmsSlow;
Biquad* highPass;
Chromosome chromosome(8, 32, 0.2, 1.2);
AmpDiff meanAmps(numInputs);
EnvelopeFollower* envIn;
Moving_Average* movAvg;
level_crossfade* xFade;
OnePole_LP* zcFilt;
OnePole_LP* rmsDiffSmooth;
Leaky_Integrator* ampDiffInt;
Adaptive_Comb* adaptive_Comb;
Variance<float> AmpVariance(numInputs);
Amplitude_Sync ampSync;
Scope scope;

AuxiliaryTask gGA;
void process_GA_background(void *);
	
bool setup(BelaContext *context, void *userData)
{
highPass = new Biquad[numInputs];
envelope = new EnvelopeFollower[numInputs];
wavesets = new Wavesets_RT_Shell[numInputs];
envIn = new EnvelopeFollower[numInputs];
movAvg = new Moving_Average[numInputs];
xFade = new level_crossfade[numInputs];
rms = new RMS[numInputs];
rmsSlow = new RMS[numInputs];
zcFilt = new OnePole_LP[numInputs];
rmsDiffSmooth = new OnePole_LP[numInputs];
specBal = new Spectral_Balance_Cheap[numInputs];
ampDiffInt = new Leaky_Integrator[numInputs];
adaptive_Comb = new Adaptive_Comb[numInputs];
weights_.resize(numInputs * 2);
ampDiffMeans.resize(numInputs * 2);

weights_ = chromosome.getInitPop();

ampSync.setWeights(weights_);

Biquad::Settings settings{
			.fs = context->audioSampleRate,
			.cutoff = gLpFreq,
			.type = Biquad::highpass,
			.q = gFilterQ,
			.peakGainDb = 0,
			};
		
				
	for (unsigned int n = 0; n < numInputs; n++) {
	
	wavesets[n].setup(context->audioSampleRate, 0.7, 10, 0.1, 2, 1, 5);
	rms[n].setup(50.0, context->audioSampleRate);
	rmsSlow[n].setup(0.2, context->audioSampleRate);
	highPass[n].setup(settings);
	envIn[n].setup(0.01, 0.15, context->audioSampleRate);
	movAvg[n].initialise(16);
	xFade[n].initialise(-37.0);
	zcFilt[n].setFc(10, context->audioSampleRate);
	rmsDiffSmooth[n].setFc(10, context->audioSampleRate);
	specBal[n].initialise(0.01, 15.0, context->audioSampleRate);
	ampDiffInt[n].init(0.8);

	float schmittLow = schmittLowThres[n];
	float schmittHigh = schmittHighThres[n];
	
	adaptive_Comb[n].init(100, 0.2, schmittLow, schmittHigh, 1, context->audioSampleRate);
	}
	
	ampSync.init(numInputs, wtSize_, context->audioSampleRate, 2.5);
	
	gGA = Bela_createAuxiliaryTask(process_GA_background, 3, "bela-process-GA");
	
	scope.setup(4, context->audioSampleRate);
	// Check that we have the same number of inputs and outputs.
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
    weights_ = chromosome.getNewFloatPop();
	ampSync.setWeights(weights_);
	float newLifeSpan = 40.0 - clamp2(mapLin(ampVarianceSum, 0, 0.3f, minLifeSpan_, maxLifeSpan_), minLifeSpan_, maxLifeSpan_);
	lifeSpan = 48000 * newLifeSpan;
	meanAmps.reset();
	ampVarianceSum = 0;
}

void render(BelaContext *context, void *userData)
{

	for(unsigned int n = 0; n < context->audioFrames; n++) {
		 float mix = 0.0;
		 
		 float balSync = clamp2(mapLin(analogRead(context, n * 0.5f, 4),  0.01f, 3.3 / 4.096, 0.0f, 1.0f), 0.0f, 1.0f);
		 if (balSync < 0.01f){
		 genCount = false;
		 } else {
			genCount = true;
		 };
		 
		 float balStretch = clamp2(mapLin(analogRead(context, n / 2, 5) ,  0.01f, 3.3 / 4.096, 0.0f, 1.0f), 0.0f, 1.0f);
		 float balSat = clamp2(mapLin(analogRead(context, n / 2, 6),  0.01f, 3.3 / 4.096, 0.0f, 1.0f), 0.0f, 1.0f);
		 //float satThresh = clamp2(mapLin(analogRead(context, n  / 2, 7),  0.01f, 3.3 / 4.096, 0.5f, 0.9f), 0.5f, 0.9f);
	

		for(unsigned int ch = 0; ch < numInputs; ch++){
			volume[ch] = mapLin(analogRead(context, n / 2, ch),  0.001f, 3.3 / 4.096, 0.0f, 1.0f);
			source[ch] = audioRead(context, n, ch);
			
			envTest[ch] = envIn[ch].process(source[ch]);
			
			_movAvg[ch] = movAvg[ch].process(source[ch]);
			
			dB[ch] = envIn[ch].getdb();
			
			float movavgFilt = xFade[ch].process(source[ch], _movAvg[ch], dB[ch]);

			dc[ch] = highPass[ch].process(movavgFilt);
			
			specBal[ch].process(delayBlend[ch]);
		
			gSpectralBalance[ch] = specBal[ch].getTendencyHZ();
			
			onePole[ch] = zcFilt[ch].process(clamp2(gSpectralBalance[ch], 30.0f, 1500.0f));
		
			gEnvelopeOutput[ch] = clamp2(mapLin(dB[ch], minDbMap[ch], -5.0f, 0.0f, 1.0f), 0.0f, 1.0f);
			
			ampSync.push(gEnvelopeOutput[ch], ch);

			meanAmps.push(gEnvelopeOutput[ch], ch);
	
			float out = ampSync.process(dc[ch], ch);

			out = Balance2(dc[ch], out, balSync);
			
			stretch_[ch] = wavesets[ch].process(dc[ch]);

			float blend = Balance2(out, stretch_[ch], balStretch);
			
			float delay_ = adaptive_Comb[ch].process(blend);
			
			delayBlend[ch] = Balance2(blend, delay_, balSat);

			rmsVal[ch] = rms[ch].process(delayBlend[ch]);
			float rmsSlowFilt = rmsSlow[ch].process(delayBlend[ch]);
			rmsValSlow[ch] = rmsDiffSmooth[ch].process(fabsf_neon(rmsSlowFilt - rmsVal[ch]));
			rmsValSlow[ch] = clamp2((float)ampDiffInt[ch].process(rmsValSlow[ch]), 0.0f, 1.0f);

			AmpVariance.push(rmsVal[ch], ch);
			adaptive_Comb[ch].setVariables(onePole[ch], rmsVal[ch], rmsValSlow[ch]);

			float gainAdjust = delayBlend[ch] * volume[ch] * gStringBal[ch];
			
			mix += gainAdjust;

			audioWrite(context, n, ch + 2, gainAdjust);
			
		}
		
		// rt_printf("Val: %f \n", dB[3]);
		

		audioWrite(context, n, 1, mix);
		
		ampSync.compute();
		
		gVarUpdateCount++;
		
		if(gVarUpdateCount >= gVarUpdateSens){
			  gVarUpdateCount = 0;
			  for(unsigned int i = 0; i < numInputs; i++){
			int	balance_ = (int) mapLin(clamp2(gSpectralBalance[i], 30.0f, 1500.0f), 40.0f, 1500.0f, 10, 1);
			  int rmsLin = mapLin(rmsVal[i], 0.0f,  0.3f, 1, 10);
			  rmsLin = clamp2((int) rmsLin, 1, 10);
			  wavesets[i].setVariables(rmsLin, 1, balance_);
			  float satThresh = clamp2(mapLin(rmsVal[i], 0.0, 0.3, 0.5f, 0.9f), 0.5f, 0.9f);
			  ampDiffInt[i].setCoeff(satThresh);
			 float rmsSlow = mapLin(sqrtf_neon(rmsValSlow[i] + 0.000001f), 0, 1.0, 0.3, 1.2);
			  wavesets[i].setAnalysisWindow(rmsSlow);
			  }
		}
		
		meanAmps.process();
		ampVarianceSum += AmpVariance.process(meanAmps.getMeanAmp());
		
		if (genCount == true){
			gGenCounter++;
			if(gGenCounter >= lifeSpan){
				Bela_scheduleAuxiliaryTask(gGA);
				gGenCounter = 0;
			}
			
		}
		
 //scope.log(rmsVal);

	}
}

void cleanup(BelaContext *context, void *userData)
{
	
}
