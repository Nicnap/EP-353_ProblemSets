#pragma once
#include <JuceHeader.h>

namespace RandomUtils
{
    inline float getRandomFloat(float range)
    {
        return juce::Random::getSystemRandom().nextFloat() * range;
    }

    inline int getRandomInt(int max)
    {
        return juce::Random::getSystemRandom().nextInt(max);
    }
}
