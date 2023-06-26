#include "SequncerComponent.h"


SequncerComponent::SequncerComponent(juce::OwnedArray<WavetableOscillator>& activeOscillators, bool& sequncerState, juce::OwnedArray<WavetableOscillator>& oscillators)
    : activeOscillators(activeOscillators),    
    startTime(juce::Time::getMillisecondCounterHiRes() * 0.001),
    sequncerState(sequncerState),
    oscillators(oscillators)
{

    auto* wavetable = new CustomWavetable(); 
    wavetable->setWavetable(SAW);
    wavetables.add(wavetable);     

    for (int i = 0; i < amountOfLinesHorizontal; ++i)
    {
        auto* line = new juce::Line<float>(0.0f, 0.0f, 0.0f, 0.0f);
        linesHorizontal.add(line);
    }

    int amountOfLinesVertical = (float) getWidth() / PIXELS_PER_UNIT; 
    amountOfLinesVertical = 30; 
    for (int i = 0; i < amountOfLinesVertical; ++i)
    {
        auto* line = new juce::Line<float>(0.0f, 0.0f, 0.0f, 0.0f);
        linesVertical.add(line);
    }


    int j = 0; 
    for (int i = numberOfMidiNotes; i > 0; --i)
    {        
        auto* label = new juce::Label(juce::String::formatted("%d", i)); 
        label->setText(juce::String::formatted("%d", i) + "  " + stringData[j], juce::dontSendNotification);
        label->setFont(juce::Font(18.0f));             
        addAndMakeVisible(label);        
        pianoNumbers.add(label);    
        ++j; 
    }     

    addAndMakeVisible(sequncerLine);
    sequncerLine.setSize(getWidth(), getHeight());
    //sequncerLine.toFront(false);
    sequncerLine.setMouseClickGrabsKeyboardFocus(false);
    sequncerLine.setInterceptsMouseClicks(false, false); 

    setWantsKeyboardFocus(true);   
    startTimer(5); 
}

void SequncerComponent::paint(juce::Graphics& g)
{
    
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::dimgrey);

    for (int i = 0; i < linesHorizontal.size(); ++i)
    {
        g.drawLine(*linesHorizontal.getUnchecked(i), 1.25f);       
    }

    g.setColour(juce::Colours::darkmagenta);

    for (int i = 0; i < linesVertical.size(); ++i)
    {
        g.drawLine(*linesVertical.getUnchecked(i), 2.f);
    }

    g.setColour(juce::Colours::skyblue);

    for (int i = 0; i < rectanglesNotesGUI.size(); ++i)
    {        
        //g.fillRect(*rectanglesNotesGUI[i]);       

        g.fillRoundedRectangle(rectanglesNotesGUI[i]->getX(), rectanglesNotesGUI[i]->getY(), rectanglesNotesGUI[i]->getWidth(), rectanglesNotesGUI[i]->getHeight(), 8.f);

        //g.fillRoundedRectangle(*rectanglesNotesGUI[i], 10.f);
        //g.fillRoundedRectangle()
        
    }   
    
    //sequncerLine.paint(g); 
}

void SequncerComponent::resized()
{
    auto X = (float)getWidth();
    auto Y = (float)getHeight();
    updateLineCoordinates(X, Y);
    updateLabelCoordinates(Y);   
    updateRectangles(Y); 
    sequncerLine.setSize(getWidth(), getHeight());
}

int SequncerComponent::clickedSlot(int clickedYslot) // same as clicked note 
{   
    auto norm = (float) clickedYslot / getHeight(); // 0 - 1
    return norm * numberOfMidiNotes + 1;    
}

int SequncerComponent::calculateRectY(int clickedNote)
{
    // Given the current note and window height calculate Y coordinate of rectangle     
    float slotHeight = (float)getHeight() / (numberOfMidiNotes);
    return (clickedNote-1) * slotHeight;     
}

int SequncerComponent::calculateRectX(int mouseX)
{
    auto stepSize = PIXELS_PER_UNIT / (float) takt / gridSize;
    auto reminder = mouseX % (int)stepSize;
    return mouseX - reminder;
}


int SequncerComponent::checkOverlap(juce::Rectangle<int> rectangleToAdd)
{    
    juce::Rectangle<int> temp;     

	for (int i = 0; i < rectanglesNotesGUI.size(); ++i)
	{
        temp = *rectanglesNotesGUI[i];
	    if (rectangleToAdd.intersectRectangle(temp))
	    {
           //rectanglesNotesGUI[i]->setX(temp.getX());
		   return i; 
		}
	}
    
    return -1; // ce je -1 potem ga dodaj, mora torej vracat -1 
}

void SequncerComponent::mouseDown(const juce::MouseEvent& event)
{
    auto mouseX = event.getMouseDownX();
    auto mouseY = event.getMouseDownY();

    auto clickedNote = clickedSlot(mouseY);
    auto rectY = calculateRectY(clickedNote);
    auto rectX = calculateRectX(mouseX); 

    int slotHeight = (float)getHeight() / (numberOfMidiNotes);
    

    // Creat new rectangle
    auto* rectangleToAdd = new juce::Rectangle<int>(rectX, rectY + 5, rectWidth, slotHeight - 8);

    // Check if appliacble to add
    int decision = checkOverlap(*rectangleToAdd);  

    if (decision != -1)
    {
        // ubistvu smo selectali tistega   
        currentlySelectedRectangle = decision;   
    }
    else 
    {        
        // tuki addam rectangle oz. green light for a new adding a new note 
        rectanglesNotesGUI.add(rectangleToAdd); 

        // create new note: 

        if (unison)
        {
            for (int i = 0; i < unison; ++i)
            {
                auto* noteToAdd = new Note(*rectangleToAdd, (88 - clickedNote + 1) + (8 * i));
                addOscillator(noteToAdd);
                notes.add(noteToAdd);
            }           
        }
        else
        {
            auto* noteToAdd = new Note(*rectangleToAdd, 88 - clickedNote + 1); // converzija v midiNote kot je na pianu
            addOscillator(noteToAdd);
            notes.add(noteToAdd);
        }

             
    }   
    repaint(); 
}

void SequncerComponent::mouseDrag(const juce::MouseEvent& event)
{
    //rectanglesNotesGUI[currentlySelectedRectangle]->setX(rectanglesNotesGUI[currentlySelectedRectangle]->getX() + event.position.getX());
    //repaint(); 


}

void SequncerComponent::setBPM(int bpm)
{
    BPM = bpm; 
}

void SequncerComponent::setGridSize(int grid)
{
    gridSize = grid; 
}

void SequncerComponent::setRectangleWidth(int width)
{
    rectWidth = width * 25; 
}

bool SequncerComponent::keyPressed(const juce::KeyPress& key)
{
    if (key.getKeyCode() == juce::KeyPress::deleteKey)
    {
        //rectanglesNotesGUI[currentlySelectedRectangle]->setBounds(0,0,0,0);
        rectanglesNotesGUI.remove(currentlySelectedRectangle, true); 
        notes.remove(currentlySelectedRectangle, true);
        repaint();
    }

    // Return true to indicate that the key press event has been consumed and should not be passed on to other components
    return true;

}

void SequncerComponent::updateRectangles(float componentHeight)
{
    int slotHeight = componentHeight / (numberOfMidiNotes);

    for (int i = 0; i < rectanglesNotesGUI.size(); ++i)
    {
        //fix y koordinate and height of rectangles to accunt for resiszing, but not the other two! 

        rectanglesNotesGUI[i]->setHeight(slotHeight-8); 
        rectanglesNotesGUI[i]->setY( + 5);
        
    }
    repaint();
}

void SequncerComponent::addOscillator(Note* noteToAdd)
{   
    for (int i = 0; i < oscillators.size(); ++i)
    {
        if (oscillators[i]->getMidiNote() == noteToAdd->getMidiNote()) // ce ja potem ze obstaja oscilator z dano frekveco
        {
            noteToAdd->setOscillatorID(i);
            return; 
        }
    }

    // else make a new oscillator: 

    auto* oscilator = new WavetableOscillator(*wavetables[0]->getWavetable(), noteToAdd->getMidiNote());
    oscilator->setSampleRate(sampleRate);
    oscilator->setLevel(0.0f); 
   
    noteToAdd->setOscillatorID(oscillators.size());
    oscillators.add(oscilator);
}


void SequncerComponent::updateLineCoordinates(float componentWidth, float componentHeight)
{   
    float stepY = componentHeight / (amountOfLinesHorizontal - 1);
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
        currentLine->setStart(100 + i * PIXELS_PER_UNIT, 0.0); // prva je delimiter od labels
        currentLine->setEnd(100 + i * PIXELS_PER_UNIT, componentHeight);
        ++i;
    }
    
}

void SequncerComponent::updateLabelCoordinates(float componentHeight)
{
    float stepY = componentHeight / numberOfMidiNotes;
    int i = 0;    

    for (auto currentLabel : pianoNumbers) 
    {
        currentLabel->setBounds(0, 5+i*stepY, 100, 15);            
        ++i; 
    }
}


void SequncerComponent::setSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}




// ====================== TIMER ==============================

void SequncerComponent::timerCallback()
{
    static unsigned long stevec = 0;      

    if (sequncerState)
    {
        sequncerLine.updatePosition(1);

        for (int i = 0; i < notes.size(); ++i)
        {
            if (notes[i]->getStartTime() == stevec)
            {
                /*activeOscillators.add(
                    oscillators[
                    notes[i]->getOscillatorID() 
                    ]
                );*/
                //activeOscillators[notes[i]->getOscillatorID()]->setLevel(0.1)

                //oscillators[notes[i]->getNotedID()]->setStatus(true);
                oscillators[notes[i]->getOscillatorID()]->setLevel(0.1);
            }
            if (notes[i]->getEndTime() == stevec)
            {

                //activeOscillators.remove(0, true);

                //activeOscillators[notes[i]->getOscillatorID()]->setLevel(0.0);

                //activeOscillators.remove(notes[i]->getOscillatorID(), true);

                //oscillators[notes[i]->getNotedID()]->setStatus(false);
                oscillators[notes[i]->getOscillatorID()]->setLevel(0.0);

                //oscillators[0]->setLevel(0.0);
            }
        }

        ++stevec;

        if (stevec > 1000)
        {
            stevec = 0;
        }
    }
    
}


