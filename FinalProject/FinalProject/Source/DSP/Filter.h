#pragma once
#include <JuceHeader.h>

class Filter
{
public:
    enum class Type { LowPass, HighPass };

    void setType(Type newType);
    float processSample(float sample);

private:
    Type type = Type::LowPass;
    float lastOutput = 0.0f;
    float alpha = 0.05f; // Simple smoothing
};
