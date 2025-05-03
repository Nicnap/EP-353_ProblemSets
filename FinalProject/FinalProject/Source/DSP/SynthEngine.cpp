#include "SynthEngine.h"

SynthEngine::SynthEngine()
{
    // Add voices
    for (int i = 0; i < 8; ++i)
        synth.addVoice(new SynthVoice());

    // Add a single sound object
    synth.addSound(new SynthSound());
}

void SynthEngine::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = sampleRate;
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SynthEngine::updateFromPlanes(const juce::Array<Plane*>& planes)
{
    if (planes.size() >= 1) handleWavePlane   (*planes[0]);
    if (planes.size() >= 2) handleEffectsPlane(*planes[1]);
    if (planes.size() >= 3) handleSpatialPlane(*planes[2]);
}

void SynthEngine::renderBlock(juce::AudioBuffer<float>& buffer,
                              int startSample,
                              int numSamples)
{
    juce::MidiBuffer dummyMidi;
    synth.renderNextBlock(buffer, dummyMidi, startSample, numSamples);
}

void SynthEngine::renderNextBlock(juce::AudioBuffer<float>& buffer,
                                  const juce::MidiBuffer& midiMessages,
                                  int startSample,
                                  int numSamples)
{
    synth.renderNextBlock(buffer, midiMessages, startSample, numSamples);
}

void SynthEngine::setRandomness(float amount)
{
    randomnessDepth = juce::jlimit(0.0f, 1.0f, amount);
    // You can iterate voices here to pass randomnessDepth to each:
    // for (int i=0; i < synth.getNumVoices(); ++i)
    //    if (auto* v = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
    //        v->setRandomnessDepth(randomnessDepth);
}

double SynthEngine::getSampleRate() const
{
    return currentSampleRate;
}

void SynthEngine::handleWavePlane(const Plane& plane)
{
    // Map plane connections → oscillator settings
    // ... your DSP code here ...
}

void SynthEngine::handleEffectsPlane(const Plane& plane)
{
    // Map plane connections → filter/chorus/etc.
    // ... your DSP code here ...
}

void SynthEngine::handleSpatialPlane(const Plane& plane)
{
    // Map plane connections → pan/reverb/etc.
    // ... your DSP code here ...
}
