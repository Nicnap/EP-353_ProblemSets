#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
// Constructor & parameter setup
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

    // ADSR defaults
    adsrParams = { 0.1f, 0.1f, 0.8f, 0.2f };

    // Reverb defaults
    reverbParams.roomSize = 0.5f;
    reverb.setParameters (reverbParams);

    // Allocate delay buffers (unused)
    int maxDelay = (int)(2.0 * sampleRate);
    delayBufferL.assign (maxDelay, 0.0f);
    delayBufferR.assign (maxDelay, 0.0f);
    writeHead    = 0;
    delaySamples = (int)(0.5 * sampleRate);

    // Initialize both channels’ filters
    for (int ch = 0; ch < 2; ++ch)
    {
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

    // Synth rendering ---
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice(i)))
        {
            voice->setADSRParameters (
                useADSR->get() ? adsrParams
                               : juce::ADSR::Parameters{0,0,1,0});
            voice->setWaveType (waveType);
        }
    synth.renderNextBlock (buffer, midi, 0, numSamples);

    // Intensity normalized
    float normI = intensity / 100.0f;

    // Reverb (stereo) ---
    if (reverbOn)
    {
        reverbParams.roomSize = 0.2f + normI * 0.8f;  // 0.2→1.0
        reverb.setParameters (reverbParams);

        juce::dsp::AudioBlock<float>        block  (buffer);
        juce::dsp::ProcessContextReplacing<float> ctx (block);
        reverb.process (ctx);
    }

    // --- 3) Delay (entirely commented out) ---
    /*
    if (delayOn)
    {
        // your old delay code here, now inert
    }
    */

    // LPF per channel, in‑place coefficient update bc static ---
    if (lpfOn)
    {
        float cutoff = 20000.0f - normI * 19800.0f; // 20k→200Hz
        for (int ch = 0; ch < 2; ++ch)
        {
            *lpfFilters[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRateHz, cutoff);
            auto* ptr = buffer.getWritePointer (ch);
            for (int i = 0; i < numSamples; ++i)
                ptr[i] = lpfFilters[ch].processSample (ptr[i]);
        }
    }

    // HPF per channel, in‑place coefficient update bc static ---
    if (hpfOn)
    {
        float cutoff = 20.0f + normI * 1980.0f; // 20→2000Hz
        for (int ch = 0; ch < 2; ++ch)
        {
            *hpfFilters[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRateHz, cutoff);
            auto* ptr = buffer.getWritePointer (ch);
            for (int i = 0; i < numSamples; ++i)
                ptr[i] = hpfFilters[ch].processSample (ptr[i]);
        }
    }

    // Distortion (stereo) ---
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
