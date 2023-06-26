#include "MainComponent.h"

MainComponent::MainComponent()
{ 
    // Toliko kolikor je razlicnih not, toliko je oscilatorjev         

    grid.addItem("1:1", 1); 
    grid.addItem("1:2", 2);
    grid.addItem("1:3", 3);
    grid.addItem("1:4", 4);
    grid.setSelectedId(1);

    takt.addItem("4/4", 1);
    takt.setSelectedId(1);

    bpm.setRange(50, 200, 1); 
    bpm.setValue(120); 

    noteLength.setRange(1, 16, 1); 
    noteLength.setValue(4); 

    addAndMakeVisible(grid);
    addAndMakeVisible(takt);
    addAndMakeVisible(bpm); 
    addAndMakeVisible(noteLength); 
    addAndMakeVisible(sequncerOnOff); 
    sequncerOnOff.setButtonText("On/Off"); 
    sequncerOnOff.setToggleState(false, juce::dontSendNotification);

    sequncerOnOff.onStateChange = [this] {sequncerState = sequncerOnOff.getToggleState(); };

    bpm.onValueChange = [this] {pointer->setBPM(bpm.getValue()); };
    noteLength.onValueChange = [this] {pointer->setRectangleWidth(noteLength.getValue()); };

    grid.addListener(this); 

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    
    addAndMakeVisible(wavetableWindow);   
    addAndMakeVisible(pointer);    

    addAndMakeVisible(viewport);
    viewport.setViewedComponent(pointer);     
    //viewport.setViewPosition(0,0);
    

    setSize(1600, 900);
}

MainComponent::~MainComponent()
{    
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{      
    pointer->setSampleRate(sampleRate); 

    forLow = new CustomWavetable(); 
    forLow->setWavetable(SIN); 

    lowF = new WavetableOscillator(*forLow->getWavetable(), 1);    
    lowF->setSampleRate(48000*10); 
    lowF->setLevel(1); 
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    bufferToFill.clearActiveBufferRegion();   

    // saveToRemove = false; 

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {        
        
        float allSamples = 0;         
        
        //for (int i = 0; i < activeOscillators.size(); ++i) // iteriram samo cez aktivne oscilatorje! 
        //    // rabil bi flag dkaj je dovlojeno brisanje -> torej to se ne sme zgoditi, ko iteriram skozi 
        //{
        //    allSamples += activeOscillators[i]->getNextSample();
        //}  

        for (int i = 0; i < oscillators.size(); ++i)
        {
            allSamples += oscillators[i]->getNextSample();// *lowF->getNextSample();
        }

        allSamples /= oscillators.size();
        leftBuffer[sample] = allSamples;
        rightBuffer[sample] = allSamples;
    }

    // saveToRemove = true; 
}

void MainComponent::releaseResources()
{

}

//void MainComponent::generateNotes()
//{
//    int startTime[6] = {10, 100, 200, 280, 330, 430};
//    int duration[6] = {90, 100, 50, 50, 100, 100};  /// To ni end time! 
//    float midiNote[6] = {76, 70, 71, 71 ,70 ,76};   
//
//    for (int i = 0; i < 6; ++i)
//    {
//        Note* nota = new Note(startTime[i], duration[i], midiNote[i]);        
//        notes.add(nota); 
//    }
//}


//void MainComponent::createWavetables()
//{
//    CustomWavetable* wavetable0 = new CustomWavetable();
//    wavetable0->setWavetable(SIN);
//    wavetables.add(wavetable0);
//}

//void MainComponent::createOscillators(double sampleRate)
//{
//    std::vector<float> oneTimeFreq;   
//    
//    oneTimeFreq.push_back(notes[0]->getNoteFrequency());
//    
//    for (int i = 0; i < notes.size(); ++i)
//    {
//        for (int j = 0; j < oneTimeFreq.size(); ++j)
//        {
//            if (oneTimeFreq[j] == notes[i]->getNoteFrequency()) break;                
//            if (j == oneTimeFreq.size() - 1)
//            {
//                oneTimeFreq.push_back(notes[i]->getNoteFrequency());
//            }            
//        }
//    }  
//
//    for (int i = 0; i < oneTimeFreq.size(); ++i)
//    {
//        WavetableOscillator* oscillator = new WavetableOscillator(*wavetables[0]->getWavetable());
//        oscillator->setID(i);
//        oscillator->setFrequency(oneTimeFreq[i], sampleRate);
//        oscillators.add(oscillator);
//    }
//
//    // set NOTE ID 
//
//    //linkNotesToOscilators(); 
//
//    notes[0]->setNoteID(0); 
//    notes[1]->setNoteID(1);
//    notes[2]->setNoteID(2);
//    notes[3]->setNoteID(2);
//    notes[4]->setNoteID(1);
//    notes[5]->setNoteID(0);
//      
//}




    /*
    static unsigned long stevec = 0; // stevec prekinitev vsakih 10 milisekund -> vec kot eno leto muske! 
    static int i = 0;
    static int j = 0;
    
    if (stevec >= duration)
    {
        stevec = 0; 
        i = 0; 
        j = 0; 
    }
     

    // namesto cez use raje po vrsti ter: morajo biti urejene po velikosti! 
    //int i = 0; 

    

    while (notes[i]->getStartTime() == stevec)
    {
        oscillators[notes[i]->getNotedID()]->setStatus(true);
        oscillators[notes[i]->getNotedID()]->setLevel(0.1);

        //activeOscillators.push_back(*oscillators[notes[i]->getNotedID()]); //funkcija vraca mesto, ki dodeli oscilatorju v arrayu
        // setOscilatorArrayPlace = activeOscillators.push_back(*oscillators[notes[i]->getNotedID()]);
        // to vrednost nato vpisemo v noto, da se potem uporbi ko se odstrani oscilator od aktivnih 

        ++i;
    }

    while (notes[j]->getEndTime() == stevec)
    {
        oscillators[notes[j]->getNotedID()]->setStatus(false);
        oscillators[notes[j]->getNotedID()]->setLevel(0.0);

        //activeOscillators.pop_back();  // activeOscillators.popFromIndex(notes[j]->getOscilatorArrayPlaceFromNote);
        // mesto/indeks kjer je trenutno aktiven oscilator je zapisan v pripadajoci noti 
        //after removing elemnt from array - if it is not the last than update all notes oscilattor positions above the removed one by subtracting one!


        ++j;
    }
    
    //++i; 
   // ++j;

    /*


    for (int i = 0; i < notes.size(); ++i)
    {
        if (notes[i]->getStartTime() / 2 == stevec)
        {
            oscillators[notes[i]->getNotedID()]->setStatus(true); 
            oscillators[notes[i]->getNotedID()]->setLevel(0.1);
        }
        if (notes[i]->getEndTime() / 2 == stevec)
        {
            oscillators[notes[i]->getNotedID()]->setStatus(false);
            oscillators[notes[i]->getNotedID()]->setLevel(0.0);

        }
    }
    
    ++stevec; 
    */



/*
    static int a = 0;
    juce::String message;
    message << (int) a;
    juce::Logger::getCurrentLogger()->writeToLog(message);
    ++a;
*/





//=========================== Drawing code =====================================// 

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    wavetableWindow.setBoundsRelative(0.02, 0.02, 2.0 / 3.0, 1.0 / 3.0);
    pointer->setBounds(0.02 * (float)getWidth(), 0.4 * (float)getHeight(), 2.0 / 3.0 * (float)getWidth() * 10, 1.0 / 3.0 * (float)getHeight() * 8);
    viewport.setBounds(0.02 * (float)getWidth(), 0.4 * (float)getHeight(), 2.0 / 3.0 * (float)getWidth(), 1.0 / 3.0 * (float)getHeight() * 1.5);

    // UI elements: 

    takt.setBoundsRelative(2.0 / 3.0 + 0.03, 1.0 / 3.0 + 0.07, 0.04, 0.04);
    grid.setBoundsRelative(2.0 / 3.0 + 0.03, 1.0 / 3.0 + 0.12, 0.04, 0.04);
    bpm.setBoundsRelative(2.0 / 3.0 + 0.03, 1.0 / 3.0 + 0.17, 0.2, 0.1);
    noteLength.setBoundsRelative(2.0 / 3.0 + 0.03, 1.0 / 3.0 + 0.22, 0.2, 0.1);
    sequncerOnOff.setBoundsRelative(2.0 / 3.0 + 0.03, 1.0 / 3.0 + 0.01, 0.04, 0.04);
}

void MainComponent::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &grid)
    {
        pointer->setGridSize(grid.getSelectedId());     

    }
}



