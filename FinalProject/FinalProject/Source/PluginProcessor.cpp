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
    // Add voices and sound
    for (int i = 0; i < 5; ++i)
        synth.addVoice (new SynthVoice());
    synth.addSound (new SynthSound());

    // ADSR toggle and Intensity parameter
    addParameter (useADSR = new juce::AudioParameterBool ("useADSR", "Use ADSR", false));
    addParameter (new juce::AudioParameterFloat ("intensity", "Intensity", 0.0f, 100.0f, 0.0f));
}

WizardWaveAudioProcessor::~WizardWaveAudioProcessor() {}

//==============================================================================
void WizardWaveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);

    // Set ADSR defaults
    adsrParams.attack  = 0.1f;
    adsrParams.decay   = 0.1f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.2f;

    // Prepare DSP chain (Reverb, Delay, LPF, HPF, Distortion)
    juce::dsp::ProcessSpec spec{ sampleRate,
                                 (uint32_t) samplesPerBlock,
                                 (uint32_t) getMainBusNumOutputChannels() };
    dspChain.prepare (spec);

    // Reverb initial settings
    reverbParams.roomSize = 0.5f;
    dspChain.get<0>().setParameters (reverbParams);

    // Delay: max 2s, start 0.5s
    auto& delay = dspChain.get<1>();
    delay.reset();
    delay.setMaximumDelayInSamples ((int)(2.0 * sampleRate));
    delay.setDelay              (sampleRate * 0.5);

    // LPF@2kHz, HPF@500Hz
    *dspChain.get<2>().coefficients = 
        *juce::dsp::IIR::Coefficients<float>::makeLowPass  (sampleRate, 2000.0f);
    *dspChain.get<3>().coefficients = 
        *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate,  500.0f);

    // Distortion curve
    dspChain.get<4>().functionToUse = [] (float x) { return std::tanh (x * 2.0f); };
}

void WizardWaveAudioProcessor::releaseResources() {}

void WizardWaveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    buffer.clear();

    // Update each voice’s ADSR and waveType
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = dynamic_cast<SynthVoice*> (synth.getVoice(i)))
        {
            v->setADSRParameters (
                useADSR->get() ? adsrParams
                               : juce::ADSR::Parameters{0,0,1,0}
            );
            v->setWaveType (waveType);
        }

    synth.renderNextBlock (buffer, midi, 0, buffer.getNumSamples());

    // DSP processing context (requires a named AudioBlock) :contentReference[oaicite:6]{index=6}
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> ctx (block);

    // Normalized intensity [0..1]
    float normI = intensity / 100.0f;

    // Reverb
    if (reverbOn)
    {
        reverbParams.roomSize = 0.2f + normI * 0.8f;
        dspChain.get<0>().setParameters (reverbParams);
        dspChain.get<0>().process (ctx);
    }

    // Delay (using five‑arg jmap<double> to avoid type conflicts) :contentReference[oaicite:7]{index=7}
    if (delayOn)
    {
        double division = juce::jmap<double>(
            normI,   // value
            0.0,1.0, // source range
            0.25,2.0 // dest range
        );
        dspChain.get<1>().setDelay (getSampleRate() * (60.0/120.0) * division);
        dspChain.get<1>().process (ctx);
    }

    // LPF
    if (lpfOn)
    {
        *dspChain.get<2>().coefficients =
            *juce::dsp::IIR::Coefficients<float>::makeLowPass (
                getSampleRate(),
                500.0f + normI * 8000.0f
            );
        dspChain.get<2>().process (ctx);
    }

    // HPF
    if (hpfOn)
    {
        *dspChain.get<3>().coefficients =
            *juce::dsp::IIR::Coefficients<float>::makeHighPass(
                getSampleRate(),
                200.0f + normI * 3000.0f
            );
        dspChain.get<3>().process (ctx);
    }

    // Distortion
    if (distortionOn)
        dspChain.get<4>().process (ctx);
}

//==============================================================================
juce::AudioProcessorEditor* WizardWaveAudioProcessor::createEditor()
{
    return new WizardWaveAudioProcessorEditor (*this);
}

void WizardWaveAudioProcessor::getStateInformation (juce::MemoryBlock&) {}
void WizardWaveAudioProcessor::setStateInformation (const void*, int) {}

//==============================================================================
// **Factory function** — must be defined so the host can create your plugin :contentReference[oaicite:8]{index=8}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WizardWaveAudioProcessor();
}
