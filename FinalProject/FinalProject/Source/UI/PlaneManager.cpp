#include "PlaneManager.h"

PlaneManager::PlaneManager()
{
    addAndMakeVisible(waveformPlane);
    addAndMakeVisible(effectsPlane);
    addAndMakeVisible(spatialPlane);
}

void PlaneManager::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlaneManager::resized()
{
    auto bounds = getLocalBounds();
    auto third = bounds.getHeight() / 3;
    waveformPlane.setBounds(bounds.removeFromTop(third));
    effectsPlane.setBounds(bounds.removeFromTop(third));
    spatialPlane.setBounds(bounds);
}

Plane* PlaneManager::getPlane(Plane::Type type)
{
    switch (type)
    {
        case Plane::Type::Waveform: return &waveformPlane;
        case Plane::Type::Effects: return &effectsPlane;
        case Plane::Type::Spatial: return &spatialPlane;
        default: return nullptr;
    }
}
