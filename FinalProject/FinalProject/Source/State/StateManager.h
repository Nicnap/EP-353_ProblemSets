#pragma once

#include <JuceHeader.h>
#include "../UI/PlaneManager.h"
#include "../DSP/SynthEngine.h"
#include "../Utility/WavWriter.h"

class StateManager
{
public:
    StateManager(PlaneManager& pm, SynthEngine& synth);

    /** Called every audio block; handles continuous mode */
    void update();

    /** Request a one-shot render on the next update() */
    void triggerOneShotGeneration();

    /** Switch between continuous vs. manual (one-shot) mode */
    void setContinuousMode(bool shouldBeContinuous);

    /** Set the global randomness amount [0..1] */
    void setRandomnessAmount(float amount);

    // —— New getters for PresetManager ——  
    bool isContinuousMode()   const { return continuousMode; }  
    float getRandomnessAmount() const { return randomnessAmount; }

private:
    void generateOneShot();

    PlaneManager&  planes;
    SynthEngine&   synthEngine;

    bool continuousMode     = true;
    bool pendingOneShot     = false;
    float randomnessAmount  = 0.3f;   // store last slider value
};
