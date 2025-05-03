#include "WaveTable.h"
#include <cmath>

Wavetable::Wavetable()
    : sineTable(
        [](float x) { return std::sin(x); },
        0.0f,
        juce::MathConstants<float>::twoPi,
        2048)
{
}

void Wavetable::setFrequency(float frequency, double sampleRate)
{
    indexIncrement = frequency * juce::MathConstants<double>::twoPi / sampleRate;
}

float Wavetable::getSample()
{
    // Prepare input and output buffers for a single sample
    float in  = static_cast<float>(currentIndex);
    float out = 0.0f;
    
    // Call the 3-argument process method: process(inputPtr, outputPtr, numSamples)
    sineTable.process(&in, &out, 1);

    // Advance the phase index
    currentIndex += indexIncrement;
    if (currentIndex >= juce::MathConstants<double>::twoPi)
        currentIndex -= juce::MathConstants<double>::twoPi;

    return out;
}
