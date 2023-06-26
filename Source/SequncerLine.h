#pragma once
#include <JuceHeader.h>

class SequncerLine : public juce::Component
{
public:

    SequncerLine()
    {
        // Set the initial position of the sequencer line
        currentX = 100;   

    }

    void paint(juce::Graphics& g) override
    {
        //g.fillAll(juce::Colours::white);
        // Set the drawing color to white
        g.setColour(juce::Colours::orange);

        // Calculate the size and position of the sequencer line
        //auto bounds = getLocalBounds();
        //auto x = bounds.getX();
        //auto y = bounds.getY();
        //auto width = bounds.getWidth();
        //auto height = bounds.getHeight();
        //g.drawLine(100, 0, 100, getHeight(), 5.0f);
        // Draw the sequencer line as a vertical line
        g.drawLine(currentX, 0, currentX, getHeight(), 2.0f);
    }

    void updatePosition(float stepSize)
    {
        // Update the position of the sequencer line
        currentX += stepSize;

        // If the sequencer line has reached the end of the component, reset it to the start
        if (currentX >= getWidth())
            currentX = 100;

        // Repaint the component to update the sequencer line position
        repaint();
    }

    void resized() override
    {        

    }

private:
    float currentX;

};