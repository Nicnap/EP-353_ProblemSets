#pragma once
#include <JuceHeader.h>
#include "Plane.h"

class PlaneManager : public juce::Component
{
public:
    PlaneManager();
    ~PlaneManager() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    Plane* getPlane(Plane::Type type);
    juce::Array<Plane*> getPlanes()      { return { &waveformPlane, &effectsPlane, &spatialPlane }; }

private:
    Plane waveformPlane { Plane::Type::Waveform };
    Plane effectsPlane  { Plane::Type::Effects  };
    Plane spatialPlane  { Plane::Type::Spatial  };
};
