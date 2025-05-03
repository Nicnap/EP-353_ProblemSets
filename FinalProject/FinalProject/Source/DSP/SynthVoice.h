#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "WaveTable.h"      // Matches on-disk filename exactly
#include "EffectsChain.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice();

    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber,
                    float velocity,
                    juce::SynthesiserSound* sound,
                    int /*currentPitchWheelPosition*/) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int /*newValue*/) override {}
    void controllerMoved  (int /*controllerNumber*/, int /*newValue*/) override {}

    // *** Correct 3-parameter override ***
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                         int startSample,
                         int numSamples) override;

private:
    juce::ADSR                envelope;
    juce::ADSR::Parameters    envParams{0.1f, 0.1f, 0.8f, 0.5f};

    Wavetable                 wavetable;       // Oscillator
    EffectsChain              effects;         // Effects chain

    double                    voiceSampleRate = 44100.0;
    float                     currentFrequency = 440.0f;
    float                     level = 0.0f;
    bool                      isActive = false;
};
