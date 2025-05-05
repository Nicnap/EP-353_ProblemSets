#include "SynthVoice.h"
#include "SynthSound.h"
#include <cmath>

//-----------------------------------------------------------------------------
SynthVoice::SynthVoice()
{
    adsr.setSampleRate (44100.0);
}

//-----------------------------------------------------------------------------
bool SynthVoice::canPlaySound (juce::SynthesiserSound* s)
{
    return dynamic_cast<SynthSound*> (s) != nullptr;
}

//-----------------------------------------------------------------------------
void SynthVoice::startNote (int midiNote, float velocity,
                            juce::SynthesiserSound*, int)
{
    auto freq      = juce::MidiMessage::getMidiNoteInHertz (midiNote);
    angleDelta     = freq / getSampleRate() * 2.0 * juce::MathConstants<double>::pi;
    level          = velocity;
    adsr.setParameters (currentADSRParams);
    adsr.noteOn();
}

//-----------------------------------------------------------------------------
void SynthVoice::stopNote (float, bool allowTailOff)
{
    adsr.noteOff();
    if (! allowTailOff || ! adsr.isActive())
        clearCurrentNote();
}

//-----------------------------------------------------------------------------
template<typename T>
static T generateWaveSample (int waveType, double phase)
{
    const double twoPi = 2.0 * juce::MathConstants<double>::pi;
    double norm = std::fmod (phase / twoPi + 1.0, 1.0);

    switch (waveType)
    {
        case 0: // none
            return T (0);

        case 1: // sine
            return T (std::sin (phase));

        case 2: // triangle
            return T (norm < 0.5
                      ? (norm * 4.0 - 1.0)
                      : (3.0 - norm * 4.0));

        case 3: // pulse 10%
            return T (norm < 0.1 ? 1.0 : -1.0);

        case 4: // square 50%
            return T (std::sin (phase) >= 0.0 ? 1.0 : -1.0);

        case 5: // saw
            return T (norm * 2.0 - 1.0);

        default:
            return T (0);
    }
}

// float-buffer version
void SynthVoice::renderNextBlock (juce::AudioBuffer<float>& buffer,
                                  int startSample, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        float env    = adsr.getNextSample();
        float wave   = generateWaveSample<float> (waveType, currentAngle);
        float sample = wave * level * env;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.addSample (ch, startSample + i, sample);

        currentAngle += angleDelta;
    }
}

// double-buffer version
void SynthVoice::renderNextBlock (juce::AudioBuffer<double>& buffer,
                                  int startSample, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        double env    = adsr.getNextSample();
        double wave   = generateWaveSample<double> (waveType, currentAngle);
        double sample = wave * level * env;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.addSample (ch, startSample + i, sample);

        currentAngle += angleDelta;
    }
}

//-----------------------------------------------------------------------------
void SynthVoice::setADSRParameters (const juce::ADSR::Parameters& p)
{
    currentADSRParams = p;
}
