#include "CustomWavetable.h"


CustomWavetable::CustomWavetable()
{    
	// Creates empty wavetable:
	wavetable.setSize(1, tableSize);
	samples = wavetable.getWritePointer(0);
	for (unsigned int i = 0; i < tableSize; ++i) {samples[i] = 0.0f;} 
}

void CustomWavetable::updateWavetableValues() // GUI implementation
{

}

void CustomWavetable::setWavetable(int set)
{
	switch (set) {
	case 0: sinWave(); break;
	case 1: sawWave(); break;
	case 2: squareWave(); break;
	default: 0;
	}
}


void CustomWavetable::setHarmonics()
{
    //int harmonics[] = { 1, 3, 5, 6, 7, 9, 13, 15 };  
    int harmonics[] = { 1, 3, 5, 7, 9, 11, 13, 15 };
    //int harmonics[] = {5};

    // 1. Naredi empty array za visje harmonike

    float samplesH[1024 + 1] = { 0 };

    // 2. Pojdi cez vse harmonike -> izracunaj njihove vrednosti s pomocjo samples wavetabla 

    for (auto harmonic : harmonics)
    {
        for (unsigned int i = 0; i < tableSize; ++i) // nafilej array harmonics z vrednosti   
        {
            auto currentIndex = harmonic * i;
            while (currentIndex >= tableSize) //veckrat moras odbit tablesSize!!
            {
                currentIndex -= tableSize;
            }

            // 3. Tu racunamo vse harmonike zato jih moramo pristevati

            samplesH[i] += samples[currentIndex];        // ne mora biti v istem for loopu, ker samples[i] pobiram za nazaj in naprej;
            // ker ne morem brati in vpisati naenkrat! 
        }
    }

    // 4. Na koncu pa prenesi vrednosti in updejtaj osnovni array -> pri tem pazi da normiras in je v mejah [-1, 1]!

    for (unsigned int i = 0; i < tableSize; ++i)
    {
        samples[i] = (samples[i] + samplesH[i]) / (juce::numElementsInArray(harmonics) + 1);

        juce::String message;
        message << (float)samples[i];
        juce::Logger::getCurrentLogger()->writeToLog(message);
    }

}


juce::AudioSampleBuffer* CustomWavetable::getWavetable()
{
	return &wavetable;
}





// PRIVATE: 

void CustomWavetable::squareWave() //setes wavetable to square wave
{	
	for (unsigned int i = 0; i < tableSize; ++i)
	{
		if (i < tableSize / 2) {
			samples[i] = 1;
		}
		else{
			samples[i] = 0;
		}	
	}
}

void CustomWavetable::sawWave() //setes wavetable to saw wave
{	
	for (unsigned int i = 0; i < tableSize; ++i)
	{
		samples[i] = (float) i / (tableSize - 1);
	}
}
/*
void CustomWavetable::sinWave() //setes wavetable to sine wave
{    
    auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1); 
    auto currentAngle = 0.0;
    for (unsigned int i = 0; i < tableSize; ++i)
    {
        auto sample = std::sin(currentAngle);                                  
        samples[i] = (float)sample;
        currentAngle += angleDelta; 


        juce::String message;
        message << (float)sample;
        juce::Logger::getCurrentLogger()->writeToLog(message);
    }
}
*/

void CustomWavetable::sinWave() //sets wavetable to sine wave
{
    auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize); //prej tableSize - 1
    auto currentAngle = 0.0;
    for (unsigned int i = 0; i < tableSize; ++i)
    {
        auto sample = std::sin(currentAngle);
        samples[i] = (float)sample;
        currentAngle += angleDelta;
    }

   // samples[tableSize-1] = samples[0]; //last value is 0 to conclude sine wave

    for (int i = 0; i < tableSize; ++i) {
        /*juce::String message;
        message << (float)samples[i];
        juce::Logger::getCurrentLogger()->writeToLog(message);*/
    }

}