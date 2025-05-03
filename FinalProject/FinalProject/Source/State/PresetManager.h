#pragma once

#include <JuceHeader.h>
#include "../UI/PlaneManager.h"
#include "StateManager.h"

class PresetManager
{
public:
    PresetManager(PlaneManager& planes, StateManager& stateMgr);

    void savePreset(const juce::File& file);
    void loadPreset(const juce::File& file);

private:
    PlaneManager&   planeManager;
    StateManager&   stateManager;
};
