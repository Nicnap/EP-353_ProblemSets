#pragma once
#include "Filter.h"

class EffectsChain
{
public:
    EffectsChain();
    float process(float sample);

private:
    Filter lowpass;
    Filter highpass;
    juce::Random random;
};
