# pultzLib — a C++ library developed for signal processing on the FAAB (feedback-actuated augmented bass), using the Bela microprocessor.

This library was developed especially for the FAAB (feedback-actuated augmented bass). It was developed on the Bela board and is geared toward CPU-effcicient and low-latency signal processing of string feedback. 

Bela: https://github.com/BelaPlatform/Bela

The folder called "Demos" contain the render files for the, currently, three algorithms used on the FAAB. Each algorithm is available to run as a separate file, while the folder named "Full" contains the code for running all three algorithms synchronously.

The code is developed for my own use and is designed to run on the Bela board using the CTAG FACE capelet, using 4 inputs and 5 outputs. Less inputs and outputs may be used.

It is assummed that an analogue control interface is used to adjust volume and processing blend. 
