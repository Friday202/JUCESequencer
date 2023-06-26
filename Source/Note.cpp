#include "Note.h"

Note::Note(juce::Rectangle<int> newRect, int midi)
{
	startTime = newRect.getX() - 100;	
	endTime = startTime + newRect.getWidth();// *(float)25 / 10;
	duration = endTime - startTime;
	midiNote = midi + 20;
}

int Note::getMidiNote()
{
	return midiNote;
}

int Note::getStartTime()
{
	return startTime;
}

int Note::getEndTime()
{
	return endTime;
}

void Note::setOscillatorID(int oscillatorID)
{
	this->oscillatorID = oscillatorID;
}

int Note::getOscillatorID()
{
	return oscillatorID; 
}


