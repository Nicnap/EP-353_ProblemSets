// SynthVoice.h
#pragma once

#include <JuceHeader.h>

class SynthVoice  : public juce::SynthesiserVoice
{
public:
    SynthVoice();

    bool canPlaySound (juce::SynthesiserSound*) override;
    void startNote     (int midiNote, float velocity,
                        juce::SynthesiserSound*, int) override;
    void stopNote      (float, bool) override;
    void pitchWheelMoved   (int)                override {}
    void controllerMoved   (int, int)          override {}

    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                          int startSample, int numSamples) override;
    void renderNextBlock (juce::AudioBuffer<double>& outputBuffer,
                          int startSample, int numSamples) override;

    void setADSRParameters (const juce::ADSR::Parameters&);
    void setMorph          (float from, float to)  { morphPos = to; }
    void setWaveType       (int newType)           { waveType = newType; }

private:
    juce::ADSR              adsr;
    juce::ADSR::Parameters  currentADSRParams;
    float                   morphPos      = 0.0f;
    double                  currentAngle  = 0.0,
                            angleDelta    = 0.0,
                            level         = 0.0;
    int                     waveType      = 1;    // 1=sine,2=tri,3=sq10%,4=sq50%,5=saw

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoice)
};
