#pragma once

#include <JuceHeader.h>

/**
 * Creates the AudioProcessorValueTreeState parameter layout for MagicSynth.
 */
inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Number of nodes in each plane
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "nodesWave",    "Wave Nodes",    3, 12, 6));
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "nodesEffects", "Effect Nodes",  1,  8, 4));
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "nodesSpatial", "Spatial Nodes", 1,  6, 3));

    // Global randomness depth
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "randomDepth",  "Randomness Depth", 0.0f, 1.0f, 0.2f));

    return { params.begin(), params.end() };
}
