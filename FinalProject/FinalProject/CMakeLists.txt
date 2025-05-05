cmake_minimum_required(VERSION 3.15)

project(WizardWave VERSION 1.0.0 LANGUAGES C CXX)

# Pull in JUCE as a subdirectory (assumes JUCE/ sits next to this file)
add_subdirectory(${CMAKE_SOURCE_DIR}/JUCE JUCE-build)

# Define the plugin target
juce_add_plugin(WizardWave
    PLUGIN_MANUFACTURER_CODE WWAV
    PLUGIN_CODE          WW01
    FORMATS              AU VST3                 # expose as both AU and VST3
    PRODUCT_NAME         "WizardWave"
    PLUGIN_IS_SYNTH                             # flag it as a synth/instrument
    NEEDS_MIDI_INPUT                            # request MIDI
    PLUGIN_CHANNEL_IO   "0->2"                  # no audio inputs, stereo output
)

# Use C++17
target_compile_features(WizardWave PRIVATE cxx_std_17)

# All source files
target_sources(WizardWave PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/SynthVoice.cpp
    Source/SynthSound.cpp
)

# Link JUCE modules
target_link_libraries(WizardWave
    PRIVATE
      juce::juce_core
      juce::juce_data_structures
      juce::juce_events
      juce::juce_graphics
      juce::juce_gui_basics
      juce::juce_audio_basics
      juce::juce_audio_processors
      juce::juce_audio_utils
	juce::juce_dsp
)

# Generate the consolidated JUCE header
juce_generate_juce_header(WizardWave)

