#pragma once

#include <JuceHeader.h>
#include "WavetableOscillator.h"
#include "CustomWavetable.h"
#include "WavetableComponent.h"
#include "Note.h"
#include "SequncerComponent.h"


class MainComponent  : public juce::AudioAppComponent, public juce::ComboBox::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;    

    //=========================== Audio thread ======================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override; 
    void comboBoxChanged(juce::ComboBox* comboBox)override;

    //==============================================================================
    //void generateNotes();
    //void createOscillators(double sampleRate);
    //void createWavetables();  

private:
    //==============================================================================         
    int duration = 600;
    
    // ON/OFF button: 
    bool sequncerState = false;

    // Active oscillators:   
    juce::OwnedArray<WavetableOscillator> activeOscillators;

    // All oscillators: 
    juce::OwnedArray<WavetableOscillator> oscillators; 

    // Low Freq oscillator:  
    CustomWavetable* forLow; 
    WavetableOscillator* lowF;

    // Components declaration:    
    WavetableComponent wavetableWindow; 
    SequncerComponent* pointer = new SequncerComponent(activeOscillators, sequncerState, oscillators);
    juce::Viewport viewport; 
    
    // UI elements: 
    
    juce::Slider bpm; 
    juce::ComboBox takt; 
    juce::ComboBox grid;
    juce::Slider noteLength; 

    juce::ToggleButton sequncerOnOff; 
        

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
