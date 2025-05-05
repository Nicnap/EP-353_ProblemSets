#pragma once

#include <JuceHeader.h>

//==============================================================================
class WizardWaveAudioProcessor  : public juce::AudioProcessor
{
public:
    WizardWaveAudioProcessor();
    ~WizardWaveAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock   (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                                  { return true; }

    //==============================================================================
    const juce::String getName() const override                      { return "WizardWave"; }
    bool acceptsMidi() const override                                { return true; }
    bool producesMidi() const override                               { return false; }
    double getTailLengthSeconds() const override                     { return 0.0; }

    //==============================================================================
    int getNumPrograms() override                                    { return 1; }
    int getCurrentProgram() override                                 { return 0; }
    void setCurrentProgram (int) override                            {}
    const juce::String getProgramName (int) override                 { return {}; }
    void changeProgramName (int, const juce::String&) override       {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    // Exposed to editor:
    juce::AudioParameterBool*    getUseADSRParam()       { return useADSR; }
    juce::ADSR::Parameters&      getADSRParameters()     { return adsrParams; }
    juce::Synthesiser&           getSynth()              { return synth; }

    // Called by editor
    void setWaveType          (int newType)    { waveType = newType; }
    void setReverbEnabled     (bool b)         { reverbOn     = b; }
    void setDelayEnabled      (bool b)         { delayOn      = b; }
    void setLPFEnabled        (bool b)         { lpfOn        = b; }
    void setHPFEnabled        (bool b)         { hpfOn        = b; }
    void setDistortionEnabled (bool b)         { distortionOn = b; }
    void setIntensity         (float i)        { intensity    = i; }

private:
    juce::Synthesiser           synth;
    juce::AudioParameterBool*   useADSR     { nullptr };
    juce::ADSR::Parameters      adsrParams;

    int   waveType    = 0;
    bool  reverbOn    = false,
          delayOn     = false,
          lpfOn       = false,
          hpfOn       = false,
          distortionOn = false;
    float intensity   = 0.0f; // 0–100

    // DSP chain: Reverb, Delay, LPF, HPF, Distortion
    using DSPChain = juce::dsp::ProcessorChain<
                        juce::dsp::Reverb,
                        juce::dsp::DelayLine<float>,
                        juce::dsp::IIR::Filter<float>,
                        juce::dsp::IIR::Filter<float>,
                        juce::dsp::WaveShaper<float>
                     >;
    DSPChain dspChain;
    juce::dsp::Reverb::Parameters reverbParams;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardWaveAudioProcessor)
};
