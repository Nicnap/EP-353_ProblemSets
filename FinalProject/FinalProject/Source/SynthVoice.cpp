// SynthVoice.cpp
#include "SynthVoice.h"
#include "SynthSound.h"
#include <cmath>

SynthVoice::SynthVoice()
{
    adsr.setSampleRate (44100.0);
}

bool SynthVoice::canPlaySound (juce::SynthesiserSound* s)
{
    return dynamic_cast<SynthSound*> (s) != nullptr;
}

void SynthVoice::startNote (int midiNote, float velocity,
                            juce::SynthesiserSound*, int)
{
    auto freq      = juce::MidiMessage::getMidiNoteInHertz (midiNote);
    angleDelta     = freq / getSampleRate() * 2.0 * juce::MathConstants<double>::pi;
    level          = velocity;
    adsr.setParameters (currentADSRParams);
    adsr.noteOn();
}

void SynthVoice::stopNote (float, bool allowTailOff)
{
    adsr.noteOff();
    if (! allowTailOff || ! adsr.isActive())
        clearCurrentNote();
}

// float version
void SynthVoice::renderNextBlock (juce::AudioBuffer<float>& buffer,
                                  int startSample, int numSamples)
{
    const double twoPi = 2.0 * juce::MathConstants<double>::pi;

    for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        // 1) generate raw waveform [-1..1] based on waveType
        double normPhase = std::fmod (currentAngle / twoPi + 1.0, 1.0);
        double base = 0.0;

        switch (waveType)
        {
            case 1: // sine
                base = std::sin (currentAngle);
                break;

            case 2: // triangle
                base = (normPhase < 0.5)
                       ? (normPhase * 4.0 - 1.0)
                       : (3.0 - normPhase * 4.0);
                break;

            case 3: // 10% pulse
                base = (normPhase < 0.1 ? 1.0 : -1.0);
                break;

            case 4: // 50% square
                base = (std::sin (currentAngle) >= 0.0 ? 1.0 : -1.0);
                break;

            case 5: // saw
                base = (normPhase * 2.0 - 1.0);
                break;

            default:
                base = std::sin (currentAngle);
                break;
        }

        // 2) apply ADSR envelope and level
        float envSample = adsr.getNextSample();
        float outSample = static_cast<float> (base * level * envSample);

        // 3) write into all channels
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.addSample (ch, startSample + sampleIndex, outSample);

        // 4) advance phase
        currentAngle += angleDelta;
    }
}

// double version
void SynthVoice::renderNextBlock (juce::AudioBuffer<double>& buffer,
                                  int startSample, int numSamples)
{
    const double twoPi = 2.0 * juce::MathConstants<double>::pi;

    for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        double normPhase = std::fmod (currentAngle / twoPi + 1.0, 1.0);
        double base = 0.0;

        switch (waveType)
        {
            case 1:
                base = std::sin (currentAngle);
                break;
            case 2:
                base = (normPhase < 0.5)
                       ? (normPhase * 4.0 - 1.0)
                       : (3.0 - normPhase * 4.0);
                break;
            case 3:
                base = (normPhase < 0.1 ? 1.0 : -1.0);
                break;
            case 4:
                base = (std::sin (currentAngle) >= 0.0 ? 1.0 : -1.0);
                break;
            case 5:
                base = (normPhase * 2.0 - 1.0);
                break;
            default:
                base = std::sin (currentAngle);
                break;
        }

        double envSample = adsr.getNextSample();
        double outSample = base * level * envSample;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.addSample (ch, startSample + sampleIndex, outSample);

        currentAngle += angleDelta;
    }
}

void SynthVoice::setADSRParameters (const juce::ADSR::Parameters& p)
{
    currentADSRParams = p;
}
