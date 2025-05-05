#pragma once

#include <JuceHeader.h>

// Forward‑declare the editor to avoid circular includes
class WizardWaveAudioProcessorEditor;

//==============================================================================
class WizardWaveAudioProcessor  : public juce::AudioProcessor
{
public:
    WizardWaveAudioProcessor();
    ~WizardWaveAudioProcessor() override;

    // Audio callbacks
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock   (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // Editor creation
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                  { return true; }

    // Boilerplate
    const juce::String getName() const override      { return "WizardWave"; }
    bool acceptsMidi() const override                { return true; }
    bool producesMidi() const override               { return false; }
    double getTailLengthSeconds() const override     { return 0.0; }
    int getNumPrograms() override                    { return 1; }
    int getCurrentProgram() override                 { return 0; }
    void setCurrentProgram (int) override            {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}
    void getStateInformation (juce::MemoryBlock&) override {}
    void setStateInformation (const void*, int) override   {}

    // Called by the editor:
    juce::AudioParameterBool* getUseADSRParam()       { return useADSR; }
    void setWaveType          (int wt)                { waveType     = wt; }
    void setReverbEnabled     (bool b)                { reverbOn     = b; }
    void setDelayEnabled      (bool b)                { delayOn      = b; }  // still unused
    void setLPFEnabled        (bool b)                { lpfOn        = b; }
    void setHPFEnabled        (bool b)                { hpfOn        = b; }
    void setDistortionEnabled (bool b)                { distortionOn = b; }
    void setIntensity         (float i)               { intensity    = i; }

private:
    // Synth core
    juce::Synthesiser           synth;
    juce::AudioParameterBool*   useADSR    { nullptr };
    juce::ADSR::Parameters      adsrParams;
    int   waveType      = 0;
    bool  reverbOn      = false,
          delayOn       = false,
          lpfOn         = false,
          hpfOn         = false,
          distortionOn  = false;
    float intensity     = 0.0f;
    float sampleRateHz  = 44100.0f;

    // Reverb
    juce::dsp::Reverb             reverb;
    juce::dsp::Reverb::Parameters reverbParams;

    // Two independent LPF and HPF filters (one per channel)
    std::array<juce::dsp::IIR::Filter<float>, 2> lpfFilters, hpfFilters;

    // Delay buffers (kept but not used)
    std::vector<float> delayBufferL, delayBufferR;
    int writeHead = 0, delaySamples = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardWaveAudioProcessor)
};
