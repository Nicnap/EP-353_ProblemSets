#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MagicSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MagicSynthAudioProcessorEditor (MagicSynthAudioProcessor&);
    ~MagicSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MagicSynthAudioProcessor& processor;

    juce::ToggleButton continuousModeButton;
    juce::Slider       randomnessSlider;
    juce::TextButton   oneShotButton;
    juce::TextButton   savePresetButton;
    juce::TextButton   loadPresetButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagicSynthAudioProcessorEditor)
};
