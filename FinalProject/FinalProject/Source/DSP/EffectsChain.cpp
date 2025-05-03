#include "EffectsChain.h"

EffectsChain::EffectsChain()
{
    lowpass.setType(Filter::Type::LowPass);
    highpass.setType(Filter::Type::HighPass);
}

float EffectsChain::process(float sample)
{
    float mod = 0.995f + (random.nextFloat() * 0.01f); // Random subtle mod
    sample = lowpass.processSample(sample * mod);
    sample = highpass.processSample(sample);
    return sample;
}
