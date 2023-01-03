# pultzLib

pultzLib is a C++ DSP library developed for my own work with the FAAB (feedback-actuated augmented bass). 

The algorithms are designed to run on the Bela board with a CTAG-cape, but, with some tweaking, should run on any platform. Similarly, while designed for signal processing on 4 inputs, the algorithms should work with higher amounts of inputs. With inputs less than 4, some of the adaptive behaviours (especially with the Amplitude Sync algorithm) will be lost. 

It is assumed that analogue controllers are controlling string gain and wet/dry mix. Currently Analogue in 0-3 are mapped to string gain, while 4-6 controls wet/dry mix, in the order of Amplitude Sync (Analogue in 4), Waveset synth (Analogue in 5) and Adaptive Comb (Analogue in 6).

Currently, I am using three algorithms, which are all included here:
Amplitude Sync: an algorithm that attempts to syng the gain of each string to the mean amplitudes of its two neighboring strings.
Waveset synthesis: Time-stretch algorithm based on Trevor Wishart's waveset synthesis algorithm.
Adaptve comb filter: Comb filter with adaptive frequency and filter feedback coefficient.

How to use on the Bela (please see https://bela.io/ for info on the Bela board):
Download the repository and make a new project in the Bela IDE. Drag all files except the folder called "Demos" to the file container on the right side of the IDE. You will need to place all files on the same level, that is, avoid dragging the folders themself into the Bela project. For running all algorithms at once, drag the render.cpp file from the folder called "Full" in the "Demos" folder. For running any single algorithm, replace the render.cpp file in the "Full" folder with its equivalent from the other folders.


