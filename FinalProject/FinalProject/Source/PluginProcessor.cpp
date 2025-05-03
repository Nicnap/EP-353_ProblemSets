#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Constructor & Destructor

MagicSynthAudioProcessor::MagicSynthAudioProcessor()
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
{
    // Initialize state and preset managers
    stateManager  = std::make_unique<StateManager>(planeManager,  synthEngine);
    presetManager = std::make_unique<PresetManager>(planeManager, *stateManager);
}

MagicSynthAudioProcessor::~MagicSynthAudioProcessor() {}

//==============================================================================
// Basic Information

const juce::String MagicSynthAudioProcessor::getName() const                { return JucePlugin_Name; }
bool MagicSynthAudioProcessor::acceptsMidi() const                         { return true; }
bool MagicSynthAudioProcessor::producesMidi() const                        { return false; }
bool MagicSynthAudioProcessor::isMidiEffect() const                        { return false; }
double MagicSynthAudioProcessor::getTailLengthSeconds() const              { return 0.0; }

//==============================================================================
// Program Management

int MagicSynthAudioProcessor::getNumPrograms()                                { return 1; }
int MagicSynthAudioProcessor::getCurrentProgram()                             { return 0; }
void MagicSynthAudioProcessor::setCurrentProgram (int)                        {}
const juce::String MagicSynthAudioProcessor::getProgramName (int)             { return {}; }
void MagicSynthAudioProcessor::changeProgramName (int, const juce::String&)   {}

//==============================================================================
// Preparation & Release

void MagicSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Prepare DSP engine
    synthEngine.prepareToPlay(sampleRate, samplesPerBlock);
}

void MagicSynthAudioProcessor::releaseResources()
{
    // (Nothing to do here currently)
}

//==============================================================================
// Bus Layout

#ifndef JucePlugin_PreferredChannelConfigurations
bool MagicSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsSynth
    // Must have some output
    return layouts.getMainOutputChannelSet() != juce::AudioChannelSet::disabled();
#else
    // Input and output must both be stereo
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())  return false;
    if (layouts.getMainInputChannelSet()  != juce::AudioChannelSet::stereo())  return false;
    return true;
#endif
}
#endif

//==============================================================================
// Audio Processing

void MagicSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    // Update continuous/one-shot logic
    stateManager->update();

    // Render synth voices
    synthEngine.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
// Editor

bool MagicSynthAudioProcessor::hasEditor() const               { return true; }
juce::AudioProcessorEditor* MagicSynthAudioProcessor::createEditor()
{
    return new MagicSynthAudioProcessorEditor (*this);
}

//==============================================================================
// State Serialization

void MagicSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // TODO: save parameters/state here
}

void MagicSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // TODO: restore parameters/state here
}

//==============================================================================
// Factory function for AU and other formats

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MagicSynthAudioProcessor();
}
