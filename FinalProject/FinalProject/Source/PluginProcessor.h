#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    Our processor now holds DSP modules for reverb, delay, filters, pan & distortion,
    plus flags to switch them on/off.
*/
class WizardWaveAudioProcessor  : public juce::AudioProcessor
{
public:
    WizardWaveAudioProcessor();
    ~WizardWaveAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor()     const override { return true;  }

    //==============================================================================
    const juce::String getName() const override                        { return "WizardWave"; }
    bool acceptsMidi() const override                                  { return true;  }
    bool producesMidi() const override                                 { return false; }
    double getTailLengthSeconds() const override                       { return 0.0;   }

    //==============================================================================
    int  getNumPrograms() override                                     { return 1;    }
    int  getCurrentProgram() override                                  { return 0;    }
    void setCurrentProgram (int) override                              {}
    const juce::String getProgramName (int) override                   { return {};   }
    void changeProgramName (int, const juce::String&) override         {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    // Exposed to editor:
    juce::AudioParameterBool* getUseADSRParam()    { return useADSR; }
    juce::ADSR::Parameters&     getADSRParameters()  { return adsrParams; }
    juce::Synthesiser&          getSynth()           { return synth; }

    // Called by editor to switch wave type (1–5)
    void setWaveType (int newType)    { waveType = newType; }

    // Called by editor to enable/disable effects
    void setReverbEnabled     (bool b) { reverbOn     = b; }
    void setDelayEnabled      (bool b) { delayOn      = b; }
    void setPanLeftEnabled    (bool b) { panLeftOn    = b; }
    void setPanRightEnabled   (bool b) { panRightOn   = b; }
    void setLPFEnabled        (bool b) { lpfOn        = b; }
    void setHPFEnabled        (bool b) { hpfOn        = b; }
    void setDistortionEnabled (bool b) { distortionOn = b; }

private:
    // your synth
    juce::Synthesiser           synth;
    juce::AudioParameterBool*   useADSR { nullptr };
    juce::ADSR::Parameters      adsrParams;

    // which waveform to generate (1–5)
    int waveType = 5;

    // effect flags
    bool reverbOn     = false,
         delayOn      = false,
         panLeftOn    = false,
         panRightOn   = false,
         lpfOn        = false,
         hpfOn        = false,
         distortionOn = false;

    // DSP processors
    juce::Reverb                reverb;
    juce::dsp::DelayLine<float> delayLine{ 44100 * 2 }; // 2‑second max
    juce::dsp::IIR::Filter<float> lpf, hpf;
    juce::dsp::ProcessorChain<juce::dsp::WaveShaper<float>> distChain;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardWaveAudioProcessor)
};
