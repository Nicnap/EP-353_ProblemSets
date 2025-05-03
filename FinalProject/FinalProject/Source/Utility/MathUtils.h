#pragma once

#include <algorithm>      // for std::min/std::max
#include <JuceHeader.h>   // for juce::MathConstants

namespace MathUtils
{
    /** Linearly interpolate between a and b by t in [0..1] */
    float lerp(float a, float b, float t);

    /** Clamp value to [minValue, maxValue] */
    float clamp(float value, float minValue, float maxValue);

    /** Convert degrees to radians */
    float toRadians(float degrees);

    /** Convert radians to degrees */
    float toDegrees(float radians);
}
