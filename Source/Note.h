#pragma once
#include <JuceHeader.h>
#include "WavetableOscillator.h"

class Note
{
public:
	//============================================================================== 
	Note(juce::Rectangle<int> newRect, int midi);

	//==============================================================================	
	int getStartTime();
	int getEndTime();
	int getMidiNote(); 

	//==============================================================================	
	void setOscillatorID(int oscillatorID);
	int getOscillatorID(); 

private:
	//==============================================================================	

	int startTime = 0;
	int duration = 0;
	int endTime = 0;
	int midiNote = 0;

	// Oscillator link:
	int oscillatorID = 0;

	// GUI 
	juce::Rectangle<int> rect; 

};

