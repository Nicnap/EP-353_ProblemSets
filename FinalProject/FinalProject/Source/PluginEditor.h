#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class WizardWaveAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WizardWaveAudioProcessorEditor (WizardWaveAudioProcessor&);
    ~WizardWaveAudioProcessorEditor() override;

    void paint     (juce::Graphics&) override;
    void resized   () override;
    void mouseDown (const juce::MouseEvent&) override;

private:
    WizardWaveAudioProcessor& processor;
    juce::TextButton adsrButton { "Toggle ADSR" };
    juce::Slider intensitySlider;

    std::vector<juce::Point<float>>       predefinedDots;
    std::vector<std::pair<size_t,size_t>> lineSegments;
    std::vector<size_t>                   selectedDotIndices;

    void processLineSegments();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardWaveAudioProcessorEditor)
};
