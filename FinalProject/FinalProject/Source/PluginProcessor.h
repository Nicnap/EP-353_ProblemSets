#pragma once

#include <JuceHeader.h>

class SynthVoice;
class SynthSound;

class WizardWaveAudioProcessor  : public juce::AudioProcessor
{
public:
    WizardWaveAudioProcessor();
    ~WizardWaveAudioProcessor() override;

    // Expose to editor
    juce::AudioParameterBool*    getUseADSRParam()    { return useADSR; }
    juce::ADSR::Parameters&      getADSRParameters()  { return adsrParams; }
    juce::Synthesiser&           getSynth()           { return synth; }

    //==============================================================================
    void prepareToPlay (double, int) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                  { return true; }

    const juce::String getName() const override      { return "WizardWave"; }
    bool acceptsMidi() const override                { return true; }
    bool producesMidi() const override               { return false; }
    bool isMidiEffect() const override               { return false; }
    double getTailLengthSeconds() const override     { return 0.0; }

    int getNumPrograms() override                    { return 1; }
    int getCurrentProgram() override                 { return 0; }
    void setCurrentProgram (int) override            {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

private:
    juce::Synthesiser        synth;
    juce::AudioParameterBool* useADSR   { nullptr };
    juce::ADSR::Parameters    adsrParams;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardWaveAudioProcessor)
};
