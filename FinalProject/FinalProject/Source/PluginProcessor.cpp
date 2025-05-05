#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "SynthSound.h"

WizardWaveAudioProcessor::WizardWaveAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    for (int i = 0; i < 5; ++i)
        synth.addVoice (new SynthVoice());

    synth.addSound (new SynthSound());

    addParameter (useADSR = new juce::AudioParameterBool ("useADSR", "Use ADSR", false));
}

WizardWaveAudioProcessor::~WizardWaveAudioProcessor() {}

void WizardWaveAudioProcessor::prepareToPlay (double sampleRate, int)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    adsrParams.attack  = 0.1f;
    adsrParams.decay   = 0.1f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.2f;
}

void WizardWaveAudioProcessor::releaseResources() {}

void WizardWaveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    buffer.clear();

    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            v->setADSRParameters (useADSR->get() ? adsrParams
                                                 : juce::ADSR::Parameters { 0,0,1,0 });

    synth.renderNextBlock (buffer, midi, 0, buffer.getNumSamples());
}

void WizardWaveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream (destData, true);
    stream.writeByte ((uint8) (useADSR->get() ? 1 : 0));
}

void WizardWaveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream (data, (size_t) sizeInBytes, false);
    uint8 v = stream.readByte();
    useADSR->setValueNotifyingHost (v ? 1.0f : 0.0f);
}

juce::AudioProcessorEditor* WizardWaveAudioProcessor::createEditor()      
{ 
    return new WizardWaveAudioProcessorEditor (*this); 
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new WizardWaveAudioProcessor();
}

