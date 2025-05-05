#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
WizardWaveAudioProcessor::WizardWaveAudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), false))
{
    for (int i = 0; i < 5; ++i)
        synth.addVoice (new SynthVoice());
    synth.addSound (new SynthSound());

    addParameter (useADSR = new juce::AudioParameterBool ("useADSR", "Use ADSR", false));
    addParameter (new juce::AudioParameterFloat ("intensity", "Intensity", 0.0f, 100.0f, 0.0f));
}

WizardWaveAudioProcessor::~WizardWaveAudioProcessor() {}

//==============================================================================
void WizardWaveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    sampleRateHz = (float) sampleRate;

    adsrParams = { 0.1f, 0.1f, 0.8f, 0.2f };

    reverbParams.roomSize = 0.5f;
    reverb.setParameters (reverbParams);

    // Delay buffers (unused)
    int maxDelay = (int)(2.0 * sampleRate);
    delayBufferL.assign (maxDelay, 0.0f);
    delayBufferR.assign (maxDelay, 0.0f);
    writeHead    = 0;
    delaySamples = (int)(0.5 * sampleRate);

    // Initialize two LPF and HPF filters (left & right) with default cutoffs
    for (int ch = 0; ch < 2; ++ch)
    {
        lpfFilters[ch].reset();
        hpfFilters[ch].reset();

        lpfFilters[ch].coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass  (sampleRateHz, 20000.0f);
        hpfFilters[ch].coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRateHz,    20.0f);
    }
}

void WizardWaveAudioProcessor::releaseResources() {}

//==============================================================================
void WizardWaveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midi)
{
    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    jassert (numChannels == 2);

    buffer.clear();

    // 1) Synth rendering
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice(i)))
        {
            voice->setADSRParameters (
                useADSR->get() ? adsrParams
                               : juce::ADSR::Parameters{0,0,1,0});
            voice->setWaveType (waveType);
        }
    synth.renderNextBlock (buffer, midi, 0, numSamples);

    float normI = intensity / 100.0f;

    // 2) Reverb
    if (reverbOn)
    {
        reverbParams.roomSize = 0.2f + normI * 0.8f;
        reverb.setParameters (reverbParams);

        juce::dsp::AudioBlock<float>        block  (buffer);
        juce::dsp::ProcessContextReplacing<float> ctx (block);
        reverb.process (ctx);
    }

    // --- 3) Delay commented out ---
    /*
    if (delayOn)
    {
        // ...
    }
    */

    // 4) LPF: sweep 20k→200Hz per channel
    if (lpfOn)
    {
        float cutoff = 20000.0f - normI * 19800.0f; // 20k→200Hz
        for (int ch = 0; ch < 2; ++ch)
        {
            // update cutoff on that channel's filter
            *lpfFilters[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowPass (
                sampleRateHz, cutoff);

            auto* ptr = buffer.getWritePointer (ch);
            for (int i = 0; i < numSamples; ++i)
                ptr[i] = lpfFilters[ch].processSample (ptr[i]);
        }
    }

    // 5) HPF: sweep 20→2000Hz per channel
    if (hpfOn)
    {
        float cutoff = 20.0f + normI * 1980.0f; // 20→2000Hz
        for (int ch = 0; ch < 2; ++ch)
        {
            *hpfFilters[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighPass (
                sampleRateHz, cutoff);

            auto* ptr = buffer.getWritePointer (ch);
            for (int i = 0; i < numSamples; ++i)
                ptr[i] = hpfFilters[ch].processSample (ptr[i]);
        }
    }

    // 6) Distortion
    if (distortionOn)
    {
        float drive = 1.0f + normI * 4.0f;
        for (int ch = 0; ch < 2; ++ch)
        {
            auto* ptr = buffer.getWritePointer (ch);
            for (int i = 0; i < numSamples; ++i)
                ptr[i] = std::tanh (ptr[i] * drive);
        }
    }
}

//==============================================================================
juce::AudioProcessorEditor* WizardWaveAudioProcessor::createEditor()
{
    return new WizardWaveAudioProcessorEditor (*this);
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WizardWaveAudioProcessor();
}
