#include "Filter.h"

void Filter::setType(Type newType)
{
    type = newType;
}

float Filter::processSample(float sample)
{
    if (type == Type::LowPass)
    {
        lastOutput += alpha * (sample - lastOutput);
        return lastOutput;
    }
    else if (type == Type::HighPass)
    {
        float hp = sample - lastOutput;
        lastOutput += alpha * (sample - lastOutput);
        return hp;
    }
    return sample;
}
