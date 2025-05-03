#include "MathUtils.h"

namespace MathUtils
{
    float lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    float clamp(float value, float minValue, float maxValue)
    {
        return std::max(minValue, std::min(value, maxValue));
    }

    float toRadians(float degrees)
    {
        return degrees * juce::MathConstants<float>::pi / 180.0f;
    }

    float toDegrees(float radians)
    {
        return radians * 180.0f / juce::MathConstants<float>::pi;
    }
}
