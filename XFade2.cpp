/***** XFade2.cpp *****/

#include "XFade2.h"
#include <cmath>


// Constructor taking arguments for sample rate and table data
XFade2::XFade2(float balance) {
	init(balance);
}

void XFade2::init(float balance)
{
	balance_ = balance;
}


// Set the frequency 
void XFade2::setBalance(float balance) {
	balance_ = balance;
	if(balance <= 0)
	balance_ = 0.0;
	if(balance >= 1)
	balance_ = 1.0;
}


// Filter function
float XFade2::process(float val_a, float val_b) {

	float out = 0;

	out = (val_a * sqrtf(1.0 - balance_)) + (val_b * sqrtf( balance_));
			
	return out;
}