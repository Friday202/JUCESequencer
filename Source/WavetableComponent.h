#pragma once
#include <JuceHeader.h>
#include "CustomWavetable.h"

class WavetableComponent : public juce::Component
{
public:
    WavetableComponent();

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
   

    

private:
    //==============================================================================
    void updateLineCoordinates(float componentWidth, float componentHeight);
    //==============================================================================

    juce::Slider s0;
    juce::Slider s1;
    int amountOfLinesHorizontal = 5;
    int amountOfLinesVertical = 5;   

    juce::OwnedArray<juce::Line<float>> linesHorizontal; 
    juce::OwnedArray<juce::Line<float>> linesVertical;
  
    juce::OwnedArray<juce::Path> points;  

    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableComponent)
};