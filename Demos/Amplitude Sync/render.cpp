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

/*
 File for running a demo of the amplitude sync algorithm
 */

#define numInputs 4 

#include <Bela.h>
#include <algorithm>
#include <libraries/Scope/Scope.h>
#include <cmath>
#include <cstdio>
#include "Wavelets_RT_Shell.h"
#include "Spectral_Balance_Cheap.h"
#include "RMS.h"
#include <libraries/Biquad/Biquad.h>
#include <libraries/GuiController/GuiController.h>
#include <libraries/math_neon/math_neon.h>
#include "EnvelopeFollower.h"
#include "Chromosome.hpp"
#include "Operators.hpp"
#include "Moving_Average.hpp"
#include "fundamentals.hpp"
#include "Utilities.hpp"
#include "OnePole_LP.h"
#include "Amplitude_Sync.hpp"
#include "adaptiveComb.hpp"
#include "Leaky_Integrator.h"
#include "OnePole_LP.h"
#include "DelayLin.h"

using namespace pultzLib;

unsigned int gAudioChannelNum; // number of audio channels to iterate over
unsigned int gAnalogChannelNum; // number of analog channels to iterate over
unsigned int gNumIns = numInputs;
unsigned int gVarUpdateSens = 64; // When to update
unsigned int gVarUpdateCount;
float lifeSpanSec = 30;
float lifeSpan = 48000 * lifeSpanSec; // Life span of GA in samples
std::vector<float> weights_; // For storing weights for GA
std::vector<float> ampDiffMeans; // Mean amplitude difference
float gSpectralBalance[numInputs]; // Sepctral balance
float out[numInputs];
float in[numInputs];
float gStretch[numInputs];
float gStringBal[numInputs] = {1.5, 1.3, 1.0, 1.0}; // String makeup gain
float source[numInputs];
float dc[numInputs];
float gLpFreq = 40; // Biquad low pass filter
float gFilterQ = 0.707; // Biquad q
float rmsVal[numInputs];
float rmsValSlow[numInputs];
float slider[numInputs] = {0.0};
float volume[numInputs] = {0.0};
float gEnvelopeOutput[numInputs] = {0.0};
float gAmpDiff[numInputs] = {0};
float weightSum; // Sum of weights
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
float schmittLowThres[numInputs] = {0.015, 0.02, 0.04, 0.06};
float schmittHighThres[numInputs] = {0.06, 0.06, 0.08, 0.12};
float schmittSmoothFreq[numInputs] = {10, 10, 10, 8};
float delayBlend[numInputs] = {0};

EnvelopeFollower* envelope;
Wavelets_RT_Shell* wavelets;
Spectral_Balance_Cheap* specBal;
RMS* rms;
RMS* rmsSlow;
Biquad* highPass;
Chromosome chromosome(8, 32, 0.2, 1.2);
AmpDiff meanAmps(gNumIns);
EnvelopeFollower* envIn;
Moving_Average* movAvg;
level_crossfade* xFade;
OnePole_LP* zcFilt;
OnePole_LP* rmsDiffSmooth;
OnePole_LP* triggerFilt;
Leaky_Integrator* ampDiffInt;
Schmitt_Trigger* schmitt_trigger;
AdaptiveComb* adaptive_Comb;
Variance<float> AmpVariance(gNumIns);
Amplitude_Sync ampSync;
//Scope scope;

AuxiliaryTask gGA;
void process_GA_background(void *);
	
bool setup(BelaContext *context, void *userData)
{
highPass = new Biquad[numInputs];
envelope = new EnvelopeFollower[numInputs];
wavelets = new Wavelets_RT_Shell[numInputs];
envIn = new EnvelopeFollower[numInputs];
movAvg = new Moving_Average[numInputs];
xFade = new level_crossfade[numInputs];
rms = new RMS[numInputs];
rmsSlow = new RMS[numInputs];
zcFilt = new OnePole_LP[numInputs];
rmsDiffSmooth = new OnePole_LP[numInputs];
specBal = new Spectral_Balance_Cheap[numInputs];
ampDiffInt = new Leaky_Integrator[numInputs];
adaptive_Comb = new AdaptiveComb[numInputs];
triggerFilt = new OnePole_LP[numInputs];
schmitt_trigger = new Schmitt_Trigger[numInputs];
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
						
	for (unsigned int n = 0; n < gNumIns; n++) {

	rms[n].setup(50.0, context->audioSampleRate);
	rmsSlow[n].setup(0.2, context->audioSampleRate);
	highPass[n].setup(settings);
	envIn[n].setup(0.01, 0.15, context->audioSampleRate);
	movAvg[n].initialise(16);
	xFade[n].initialise(-37.0);
	zcFilt[n].setFc(10, context->audioSampleRate);
	rmsDiffSmooth[n].setFc(10, context->audioSampleRate);
	specBal[n].initialise(0.01, 15.0, context->audioSampleRate);
	ampDiffInt[n].init(0.5);
	
	float schmittLow = schmittLowThres[n];
	float schmittHigh = schmittHighThres[n];
	
	adaptive_Comb[n].init(100, 0.2, schmittLow, schmittHigh, 1, context->audioSampleRate);
	triggerFilt[n].setFc(schmittSmoothFreq[n], context->audioSampleRate);
	schmitt_trigger[n].init(schmittLow, schmittHigh);
	}
	
	ampSync.init(gNumIns, wtSize_, context->audioSampleRate, 2.5);
	
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

/*
 Thread for running GA without interrupting audio processing
 */
void process_GA_background(void *){
	ampVarianceSum /= lifeSpan; // Mean of amplitude variance across all strings
	ampVarianceSum = clamp2(ampVarianceSum, 0.000001f, 0.3f); // Constrain value
	ampDiffMeans = meanAmps.getDiffs(); // Mean of amplitude differences
//	rt_printf("Val: %f, %f, %f, %f, %f, %f, %f, %f \n", ampDiffMeans[0], ampDiffMeans[1], ampDiffMeans[2], ampDiffMeans[3],ampDiffMeans[4], ampDiffMeans[5], ampDiffMeans[6], ampDiffMeans[7] );
	chromosome.select(ampDiffMeans);
    chromosome.xOver();
    chromosome.mutation();
    weights_ = chromosome.getNewFloatPop();
//    rt_printf("Val: %f, %f, %f, %f, %f, %f, %f, %f \n", weights_[0], weights_[1], weights_[2], weights_[3], weights_[4], weights_[5], weights_[6], weights_[7] );
	ampSync.setWeights(weights_);
    /* New life span equals the inverse of the mean of amplitude variance*/
	float newLifeSpan = 40.0 - clamp2(mapLin(ampVarianceSum, 0, 0.3f, minLifeSpan_, maxLifeSpan_), minLifeSpan_, maxLifeSpan_);
	lifeSpan = 48000 * newLifeSpan; // Set new life span in samples
	meanAmps.reset(); // Reset mean amplitudes
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
		 
		 float balStretch = clamp2(mapLin(analogRead(context, n * 0.5f, 5) ,  0.01f, 3.3 / 4.096, 0.0f, 1.0f), 0.0f, 1.0f);
		 float balSat = clamp2(mapLin(analogRead(context, n * 0.5f, 6),  0.01f, 3.3 / 4.096, 0.0f, 1.0f), 0.0f, 1.0f);

		for(unsigned int ch = 0; ch < gNumIns; ch++){
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
			
			gAmpDiff[ch] = ampSync.getAmpDiff(ch);

			out = Balance2(dc[ch], out, balSync);
			
			rmsVal[ch] = rms[ch].process(delayBlend[ch]);

			AmpVariance.push(rmsVal[ch], ch);
			adaptive_Comb[ch].setVariables(onePole[ch], rmsVal[ch], rmsValSlow[ch]);

			float gainAdjust = out * volume[ch] * gStringBal[ch];
			
			mix += gainAdjust;

			audioWrite(context, n, ch + 2, gainAdjust);
			
		}
		

		audioWrite(context, n, 1, mix);
		
		ampSync.compute();
        
		meanAmps.process();
		ampVarianceSum += AmpVariance.process(meanAmps.getMeanAmp());
		
		if (genCount == true){
			gGenCounter++;
			if(gGenCounter > lifeSpan - 1){
				Bela_scheduleAuxiliaryTask(gGA);
				gGenCounter = 0;
			}
			
		}
		
// scope.log(gAmpDiff);

	}
}

void cleanup(BelaContext *context, void *userData)
{
	
}
