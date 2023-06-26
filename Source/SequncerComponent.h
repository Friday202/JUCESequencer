#pragma once
#include <JuceHeader.h>
#include "Note.h"
#include "WavetableOscillator.h"
#include "CustomWavetable.h"
#include "SequncerLine.h"

#define PIXELS_PER_UNIT 400
#define SIN 0
#define SAW 1
#define SQUARE 2
#define TRIANGLE 3
#define NOISE 4

class SequncerComponent : public juce::Component, juce::Timer
{
public:    
    //==============================================================================
    SequncerComponent(juce::OwnedArray<WavetableOscillator>& activeOscillators, bool& sequncerState, juce::OwnedArray<WavetableOscillator>& oscillators);

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    bool keyPressed(const juce::KeyPress& key) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void timerCallback() override;

    //==============================================================================
    void setBPM(int bpm);
    void setGridSize(int grid);    
    void setRectangleWidth(int width);        
    void setSampleRate(double sampleRate); 

private:
    //==============================================================================
    void updateLineCoordinates(float componentWidth, float componentHeight);
    void updateLabelCoordinates(float componentHeight);
    int clickedSlot(int clickedYslot);
    int calculateRectY(int clickedNote);
    int calculateRectX(int mouseX);
    int checkOverlap(juce::Rectangle<int> rectangleToAdd);
    void updateRectangles(float componentHeight);
    void addOscillator(Note* noteToAdd);

    // Variables: 

    int currentlySelectedRectangle = 0;   
    int numberOfMidiNotes = 88;
    int amountOfLinesHorizontal = numberOfMidiNotes + 1;
    int BPM = 60;
    int takt = 4;
    int gridSize = 1;
    int rectWidth = 100;
    double sampleRate = 44100.L;
    
    int unison = 0; 

    double startTime;   

    //Other: 
    int beatPeriod = 60000 / BPM; // beat period in milliseconds
    int interruptsPerBeat = beatPeriod / 10; // number of interrupts per beat period
    int interruptCounter = 0; // counter variable

    // On/Off button: 
    bool& sequncerState;

    // Sequncer line: 
    SequncerLine sequncerLine;

    // All oscillators: 
    juce::OwnedArray<WavetableOscillator>& oscillators;

    // Wavetables:
    juce::OwnedArray<CustomWavetable> wavetables;     

    // Active oscillators:
    juce::OwnedArray<WavetableOscillator>& activeOscillators;

    // Notes representation:
    juce::OwnedArray<juce::Rectangle<int>> rectanglesNotesGUI; 
    juce::OwnedArray<Note> notes;

    // GUI:
    juce::OwnedArray<juce::Line<float>> linesHorizontal;
    juce::OwnedArray<juce::Line<float>> linesVertical;
    juce::OwnedArray<juce::Line<float>> linesSupport;
    juce::OwnedArray<juce::Label> pianoNumbers;           
    juce::String stringData[88] =
    {
        "C8",
        "B7",
        "A#7",
        "A7",
        "G#7",
        "G7",
        "F#7",
        "F7",
        "E7",
        "D#7",
        "D7",
        "C#7",
        "C7",
        "B6",
        "A#6",
        "A6",
        "G#6",
        "G6",
        "F#6",
        "F6",
        "E6",
        "D#6",
        "D6",
        "C#6",
        "C6",
        "B5",
        "A#5",
        "A5",
        "G#5",
        "G5",
        "F#5",
        "F5",
        "E5",
        "D#5",
        "D5",
        "C#5",
        "C5",
        "B4",
        "A#4",
        "A4",
        "G#4",
        "G4",
        "F#4",
        "F4",
        "E4",
        "D#4",
        "D4",
        "C#4",
        "C4",
        "B3",
        "A#3",
        "A3",
        "G#3",
        "G3",
        "F#3",
        "F3",
        "E3",
        "D#3",
        "D3",
        "C#3",
        "C3",
        "B2",
        "A#2",
        "A2",
        "G#2",
        "G2",
        "F#2",
        "F2",
        "E2",
        "D#2",
        "D2",
        "C#2",
        "C2",
        "B1",
        "A#1",
        "A1",
        "G#1",
        "G1",
        "F#1",
        "F1",
        "E1",
        "D#1",
        "D1",
        "C#1",
        "C1",
        "B0",
        "A#0",
        "A0"
    };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequncerComponent)
};