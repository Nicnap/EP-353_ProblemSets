#pragma once

#include <JuceHeader.h>

/** 
    A simple wavetable oscillator using JUCE's LookupTableTransform.
*/
class Wavetable
{
public:
    Wavetable();
    void setFrequency(float frequency, double sampleRate);
    float getSample();

private:
    juce::dsp::LookupTableTransform<float> sineTable;
    double currentIndex = 0.0;
    double indexIncrement = 0.0;
};
