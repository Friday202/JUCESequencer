#pragma once
#include <JuceHeader.h>

class WavetableOscillator 
{
public:
	//==============================================================================
	WavetableOscillator(const juce::AudioSampleBuffer& wavetableToUse, int midiNote);
	~WavetableOscillator();

	//==============================================================================
	void setSampleRate(float sampleRate);
	void setLevel(float level); 
	float getNextSample() noexcept;
	float getTableDelta(); 

	bool isActive(); 

	void setStatus(bool flag);
	void setID(int ID); 
	int getMidiNote();
	

private:
	void convertToFrequency();
	// The task of ADSR is to only generate output level between 0 - 1 at any given time

	struct ADSR {

		float attackTime;
		float decayTime;
		float releaseTime;

		float attackLevel;
		float sustainLevel;

		float startTime;
		float stopTime;

		bool playing;

		ADSR(); 
		float getLevel(double time);
	};

	//==============================================================================		
	const juce::AudioSampleBuffer& wavetable;
	const int tableSize;

	float currentIndex = 0.0f, tableDelta = 0.0f;
	const float* table;
	float level = 0.0f;	
	bool status = false; //active 
	
	int midiNote = 0; 
	float frequency = 0.0;
};

