#pragma once

#include <JuceHeader.h>
#include "../UI/Plane.h"
#include "SynthVoice.h"
#include "SynthSound.h"

/**
    A simple wrapper around juce::Synthesiser that manages voices,
    plane–to–parameter mapping, and one‐shot/offline rendering.
*/
class SynthEngine
{
public:
    SynthEngine();
    ~SynthEngine() = default;

    /** Prepare internal synth for realtime or offline rendering */
    void prepareToPlay(double sampleRate, int samplesPerBlock);

    /**
       Update all parameters (oscillator shapes, filters, spatial) based
       on the three UI planes.
    */
    void updateFromPlanes(const juce::Array<Plane*>& planes);

    /** Render an offline block without MIDI (for one‐shot export) */
    void renderBlock(juce::AudioBuffer<float>& buffer, int startSample, int numSamples);

    /**
       Render into a live buffer with MIDI input (called by Processor)
    */
    void renderNextBlock(juce::AudioBuffer<float>& buffer,
                         const juce::MidiBuffer& midiMessages,
                         int startSample,
                         int numSamples);

    /** Control the global randomness depth */
    void setRandomness(float amount);

    /** Query the current sample rate */
    double getSampleRate() const;

private:
    void handleWavePlane(const Plane& plane);
    void handleEffectsPlane(const Plane& plane);
    void handleSpatialPlane(const Plane& plane);

    juce::Synthesiser synth;
    double            currentSampleRate = 44100.0;
    float             randomnessDepth   = 0.2f;
};
