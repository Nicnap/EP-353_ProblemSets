#include "SynthVoice.h"
#include "SynthSound.h"

SynthVoice::SynthVoice()
{
    envelope.setParameters(envParams);
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber,
                           float velocity,
                           juce::SynthesiserSound* /*sound*/,
                           int /*currentPitchWheelPosition*/)
{
    currentFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    wavetable.setFrequency(currentFrequency, voiceSampleRate);
    level      = velocity;
    isActive   = true;
    envelope.noteOn();
}

void SynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    envelope.noteOff();
    if (!allowTailOff || !envelope.isActive())
        clearCurrentNote();
    isActive = false;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                 int startSample,
                                 int numSamples)
{
    if (!isActive)
        return;

    for (int i = 0; i < numSamples; ++i)
    {
        float oscSample   = wavetable.getSample();
        float envValue    = envelope.getNextSample();
        float sampleValue = oscSample * level * envValue;
        float processed   = effects.process(sampleValue);

        for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
            outputBuffer.addSample(ch, startSample + i, processed);
    }
}
