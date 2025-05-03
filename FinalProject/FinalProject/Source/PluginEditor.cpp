#include "PluginEditor.h"
#include "PluginProcessor.h"

MagicSynthAudioProcessorEditor::MagicSynthAudioProcessorEditor (MagicSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (900, 700);
    addAndMakeVisible(processor.planeManager);

    // Continuous Mode toggle
    addAndMakeVisible(continuousModeButton);
    continuousModeButton.setButtonText("Continuous Mode");
    continuousModeButton.setToggleState(true, juce::dontSendNotification);
    continuousModeButton.onClick = [this]()
    {
        processor.stateManager->setContinuousMode(continuousModeButton.getToggleState());
    };

    // Randomness slider
    addAndMakeVisible(randomnessSlider);
    randomnessSlider.setRange(0.0, 1.0, 0.01);
    randomnessSlider.setValue(0.3);
    randomnessSlider.setTextValueSuffix(" randomness");
    randomnessSlider.onValueChange = [this]()
    {
        processor.stateManager->setRandomnessAmount((float)randomnessSlider.getValue());
    };

    // One-shot generation
    addAndMakeVisible(oneShotButton);
    oneShotButton.setButtonText("Generate One-Shot");
    oneShotButton.onClick = [this]()
    {
        processor.stateManager->triggerOneShotGeneration();
    };

    // Preset Save (always to Documents/MagicSynthPreset.preset)
    addAndMakeVisible(savePresetButton);
    savePresetButton.setButtonText("Save Preset");
    savePresetButton.onClick = [this]()
    {
        auto file = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                          .getChildFile("MagicSynthPreset.preset");
        processor.presetManager->savePreset(file);
    };

    // Preset Load (always from Documents/MagicSynthPreset.preset)
    addAndMakeVisible(loadPresetButton);
    loadPresetButton.setButtonText("Load Preset");
    loadPresetButton.onClick = [this]()
    {
        auto file = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                          .getChildFile("MagicSynthPreset.preset");
        if (file.existsAsFile())
            processor.presetManager->loadPreset(file);
    };
}

MagicSynthAudioProcessorEditor::~MagicSynthAudioProcessorEditor() {}

void MagicSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkslategrey);
}

void MagicSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    processor.planeManager.setBounds(area.removeFromTop(getHeight() - 120));

    auto controlArea = area.reduced(10);
    continuousModeButton .setBounds(controlArea.removeFromTop(30).reduced(0, 4));
    randomnessSlider     .setBounds(controlArea.removeFromTop(60).reduced(0, 4));
    oneShotButton        .setBounds(controlArea.removeFromTop(30).reduced(0, 4));
    savePresetButton     .setBounds(controlArea.removeFromTop(30).reduced(0, 4));
    loadPresetButton     .setBounds(controlArea.removeFromTop(30).reduced(0, 4));
}
