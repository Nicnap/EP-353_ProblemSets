#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "SynthSound.h"   // ← ensure SynthSound is visible

//==============================================================================
WizardWaveAudioProcessor::WizardWaveAudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withOutput ("Output",  juce::AudioChannelSet::stereo(), true)
                        .withInput  ("Input",   juce::AudioChannelSet::stereo(), false))
{
    for (int i = 0; i < 5; ++i)
        synth.addVoice (new SynthVoice());
    synth.addSound (new SynthSound());

    addParameter (useADSR = new juce::AudioParameterBool ("useADSR", "Use ADSR", false));
}

WizardWaveAudioProcessor::~WizardWaveAudioProcessor() {}

//==============================================================================
void WizardWaveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);

    adsrParams.attack  = 0.1f;
    adsrParams.decay   = 0.1f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.2f;

    // Reverb
    juce::Reverb::Parameters rp;
    rp.roomSize = 0.5f;  rp.wetLevel = 0.3f;
    reverb.setParameters (rp);

    // Delay (max 2s), set to 0.5s
    delayLine.reset();
    delayLine.setDelay (sampleRate * 0.5f);

    // Filters: use public `coefficients` pointer
    *lpf.coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowPass  (sampleRate, 2000.0f);
    *hpf.coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate,  500.0f);

    // Distortion
    auto& shaper = distChain.get<0>();
    shaper.functionToUse = [] (float x) { return std::tanh (x * 2.0f); };
}

void WizardWaveAudioProcessor::releaseResources() {}

//==============================================================================
void WizardWaveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    buffer.clear();

    // Configure each voice
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = dynamic_cast<SynthVoice*> (synth.getVoice(i)))
        {
            v->setADSRParameters ( useADSR->get() ? adsrParams
                                                   : juce::ADSR::Parameters { 0,0,1,0 } );
            v->setWaveType (waveType);
        }

    // Render synth into buffer
    synth.renderNextBlock (buffer, midi, 0, buffer.getNumSamples());

    // Wrap JUCE DSP blocks
    juce::dsp::AudioBlock<float>          block  (buffer);
    juce::dsp::ProcessContextReplacing<float> ctx (block);

    if (lpfOn)        lpf.process    (ctx);
    if (hpfOn)        hpf.process    (ctx);
    if (distortionOn) distChain.process (ctx);

    if (delayOn)
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* data = buffer.getWritePointer (ch);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                float delayed = delayLine.popSample (ch) * 0.5f;
                delayLine.pushSample (ch, data[i] + delayed);
                data[i] += delayed;
            }
        }
    }

    if (reverbOn)
        reverb.processStereo (buffer.getWritePointer(0),
                              buffer.getWritePointer(1),
                              buffer.getNumSamples());

    if (panLeftOn || panRightOn)
    {
        float pan = panLeftOn  ? -0.5f
                  : panRightOn ?  0.5f
                  : 0.0f;

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float l = buffer.getSample (0, i) * (1.0f - pan);
            float r = buffer.getSample (1, i) * (1.0f + pan);
            buffer.setSample (0, i, l);
            buffer.setSample (1, i, r);
        }
    }
}

//==============================================================================
juce::AudioProcessorEditor* WizardWaveAudioProcessor::createEditor()
{
    return new WizardWaveAudioProcessorEditor (*this);
}

// remove the extra hasEditor() definition here — it's already inline in the header

void WizardWaveAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void WizardWaveAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WizardWaveAudioProcessor();
}
