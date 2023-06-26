#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator(const juce::AudioSampleBuffer& wavetableToUse, int midiNote)
    : wavetable(wavetableToUse),
    tableSize(wavetable.getNumSamples() - 1)
{
   jassert(wavetable.getNumChannels() == 1);  
   table = wavetable.getReadPointer(0);
   this->midiNote = midiNote; 
   convertToFrequency(); 
}

WavetableOscillator::~WavetableOscillator() {
   
}

void WavetableOscillator::setSampleRate(float sampleRate)
{
	auto tableSizeOverSampleRate = (float) tableSize / sampleRate;
	tableDelta = frequency * tableSizeOverSampleRate;    
}

void WavetableOscillator::setLevel(float level)
{
    this->level = level; 
}

float WavetableOscillator::getNextSample() noexcept
{
    //auto tableSize = (unsigned int)wavetable.getNumSamples();
    //static int a = 0; 

    //jassert(a < 28795); // do 28810 at 28800

    auto index0 = (unsigned int)currentIndex;             
    auto index1 = index0 + 1;

    auto frac = currentIndex - (float)index0;  

    //jassert(currentIndex <= 1024);
    //jassert(currentIndex > 0);
    //jassert(currentIndex < 0);

    auto value0 = table[index0];
    auto value1 = table[index1];

    auto currentSample = value0 + frac * (value1 - value0);

    if ((currentIndex += tableDelta) > (float)tableSize)  
        currentIndex -= (float)tableSize;
    //++a; 
    return currentSample * level;
}

float WavetableOscillator::getTableDelta()
{
    return tableDelta;
}

bool WavetableOscillator::isActive()
{
    return status;
}

void WavetableOscillator::setStatus(bool flag)
{
    status = flag;
    //level = 0.1; 
}

//void WavetableOscillator::setID(int ID)
//{
  //  oscillatorID = ID; 
//}

int WavetableOscillator::getMidiNote()
{
    return midiNote;
}

void WavetableOscillator::convertToFrequency()
{
    frequency = 440.f * std::pow(2.0, (midiNote - 69.0) / 12.0);
}









WavetableOscillator::ADSR::ADSR() {

    attackTime = 0.01;
    decayTime = 0.01;
    releaseTime = 0.01;
    attackLevel = 1.0;
    sustainLevel = 0.8;
    playing = false; 
 }

float WavetableOscillator::ADSR::getLevel(double time) //timer time when you press play button! 
{
        float level = 0;

        if (playing)
        {
            // ADSR



        }


        return level; 
}
    /*
    void noteOn(double timeOn) 
    {
        startTime = timeOn; 
        playing = true; 
    }

    void noteOff(double timeOff)
    {
        stopTime = timeOff; 
        playing = false; 
    }

    */
