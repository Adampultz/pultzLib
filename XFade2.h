/***** XFade2.h *****/

#pragma once

class XFade2 {
public:
	XFade2() {}												
	XFade2(float balance);
			  
	void init(float balance); 		
	
	void setBalance(float balance);
	
	float process(float val_a, float val_b);			
	
	~XFade2() {}			

private:
	
	float balance_;
};