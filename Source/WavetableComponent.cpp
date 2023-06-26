#pragma once
#include "WavetableComponent.h"

WavetableComponent::WavetableComponent()
{
    addAndMakeVisible(s0);
    addAndMakeVisible(s1);

    s0.setRange(0, 1024);
    s1.setRange(-1, 1);


    for (int i = 0; i < amountOfLinesHorizontal; ++i)
    {
        auto* line = new juce::Line<float> (0.0f, 0.0f, 0.0f, 0.0f);      
        linesHorizontal.add(line);
    }

    for (int i = 0; i < amountOfLinesVertical; ++i)
    {
        auto* line = new juce::Line<float>(0.0f, 0.0f, 0.0f, 0.0f);   
        linesVertical.add(line);
    }

}

void WavetableComponent::paint(juce::Graphics& g)
{    
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::aqua);
    for (int i = 0; i < linesVertical.size(); ++i)
    {
        g.drawLine(*linesVertical.getUnchecked(i), 1.25f);
        g.drawLine(*linesHorizontal.getUnchecked(i), 1.25f);
    }
   // for (auto i : points)
   // {
   //   g.fillPath(*i);
   // }
    
}

void WavetableComponent::resized() 
{
    auto X = (float)getWidth();
    auto Y = (float)getHeight();

    //s0.setBounds(20, 20, getWidth() - 100, 20);
    //s1.setBounds(20, 80, getWidth() - 100, 20);

    updateLineCoordinates(X, Y); 

}

void WavetableComponent::mouseDown(const juce::MouseEvent& event)
{ 
    //CustomWavetable::example(getMouseXYRelative());
    
    /*
    auto pointPressed = getMouseXYRelative(); 
    
    auto pendingIndex = (float) pointPressed.getX() / getWidth() * 1024;
    auto pendingValue = -(((float) pointPressed.getY() / getHeight()) * 2 - 1);
    
    s0.setValue(pendingIndex);
    s1.setValue(pendingValue);

    auto* pointToAdd = new juce::Path;
    pointToAdd->addTriangle(pointPressed.getX(),
        pointPressed.getY() - (float)getHeight() / 6,
        pointPressed.getX() + (float)getWidth() / 6,
        pointPressed.getY() + (float)getHeight() / 6,
        pointPressed.getX() - (float)getWidth() / 6,
        pointPressed.getY() + (float)getHeight() / 6);
                        
    points.add(pointToAdd);
    */
}



void WavetableComponent::updateLineCoordinates(float componentWidth, float componentHeight)
{
    float stepX = componentWidth / (amountOfLinesHorizontal-1);
    float stepY = componentHeight / (amountOfLinesVertical-1);
    int i = 0; 

    for (auto currentLine : linesHorizontal)
    {        
        currentLine->setStart(0.0, 0.0 + i * stepY);
        currentLine->setEnd(componentWidth, 0.0 + i * stepY);
        ++i; 
    }
    i = 0; 
    for (auto currentLine : linesVertical)
    {
        currentLine->setStart(0.0 + i * stepX, 0.0);
        currentLine->setEnd(0.0 + i * stepX, componentHeight);
        ++i; 
    }
}