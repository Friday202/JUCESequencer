#pragma once
#include <JuceHeader.h>

class CustomWavetable
{
public:
	//==============================================================================
	CustomWavetable();	

	//==============================================================================
	void updateWavetableValues();
	void setWavetable(int set); 
	juce::AudioSampleBuffer* getWavetable();
	void setHarmonics();	

private:
	//==============================================================================
	void sawWave();
	void sinWave();
	void squareWave();

	//==============================================================================
	const unsigned int tableSize = 1024; 
	juce::AudioSampleBuffer wavetable;
	float* samples;		

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomWavetable)
};

