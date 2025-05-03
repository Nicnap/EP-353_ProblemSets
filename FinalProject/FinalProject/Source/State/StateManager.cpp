#include "StateManager.h"

StateManager::StateManager(PlaneManager& pm, SynthEngine& synth)
    : planes(pm), synthEngine(synth)
{
}

void StateManager::update()
{
    if (continuousMode)
        synthEngine.updateFromPlanes(planes.getPlanes());

    if (pendingOneShot)
    {
        generateOneShot();
        pendingOneShot = false;
    }
}

void StateManager::triggerOneShotGeneration()
{
    pendingOneShot = true;
}

void StateManager::setContinuousMode(bool shouldBeContinuous)
{
    continuousMode = shouldBeContinuous;
}

void StateManager::setRandomnessAmount(float amount)
{
    randomnessAmount = juce::jlimit(0.0f, 1.0f, amount);
    synthEngine.setRandomness(randomnessAmount);
}

void StateManager::generateOneShot()
{
    const double sr = synthEngine.getSampleRate();
    int lengthInSamples = int (5.0 * sr);
    juce::AudioBuffer<float> buffer(2, lengthInSamples);
    buffer.clear();

    synthEngine.renderBlock(buffer, 0, lengthInSamples);

    auto file = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                    .getChildFile("MagicSynthOneShot.wav");
    WavWriter::writeToWavFile(file, buffer, sr);
}
