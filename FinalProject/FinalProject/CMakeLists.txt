cmake_minimum_required(VERSION 3.16)

project(MagicSynth VERSION 0.1.0 LANGUAGES C CXX)

# -----------------------------
# 1) Tell CMake where JUCE is:
# -----------------------------
# Make sure you've done:
#   git clone https://github.com/juce-framework/JUCE.git JUCE
set(JUCE_DIR "${CMAKE_SOURCE_DIR}/JUCE" CACHE PATH "JUCE root directory")

# -----------------------------
# 2) Turn off examples/tools/extras
# -----------------------------
option(JUCE_BUILD_EXAMPLES    OFF)
option(JUCE_BUILD_TOOLS       OFF)
option(JUCE_BUILD_EXTRAS      OFF)

# -----------------------------
# 3) Add JUCE
# -----------------------------
add_subdirectory(${JUCE_DIR} JUCE)

# -----------------------------
# 4) Declare your plugin
# -----------------------------
juce_add_plugin(MagicSynth
    COMPANY_NAME            "YourCompany"
    IS_SYNTH                TRUE
    NEEDS_MIDI_INPUT        TRUE
    NEEDS_MIDI_OUTPUT       FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Syn1
    PLUGIN_CODE             Ms01
    FORMATS                 VST3 AU
    PRODUCT_NAME            "MagicSynth"
)

# ----------------------------------
# 5) Generate the umbrella header
#    This creates JuceHeader.h for you
# ----------------------------------
juce_generate_juce_header(MagicSynth)

# ----------------------------------
# 6) Make sure the JUCE modules
#    folder is on your include path
# ----------------------------------
target_include_directories(MagicSynth PRIVATE
    ${JUCE_DIR}/modules
)

# ----------------------------------
# 7) List all your source files
# ----------------------------------
target_sources(MagicSynth PRIVATE
    # Core plugin
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h

    # DSP
    Source/DSP/SynthVoice.cpp
    Source/DSP/SynthVoice.h
    Source/DSP/SynthSound.cpp
    Source/DSP/SynthSound.h
    Source/DSP/Wavetable.cpp
    Source/DSP/Wavetable.h
    Source/DSP/EffectsChain.cpp
    Source/DSP/EffectsChain.h
    Source/DSP/Filter.cpp
    Source/DSP/Filter.h
    Source/DSP/SynthEngine.cpp
    Source/DSP/SynthEngine.h


    # UI
    Source/UI/Node.cpp
    Source/UI/Node.h
    Source/UI/Plane.cpp
    Source/UI/Plane.h
    Source/UI/PlaneManager.cpp
    Source/UI/PlaneManager.h
    Source/UI/MagicTrail.cpp
    Source/UI/MagicTrail.h

    # State
    Source/State/StateManager.cpp
    Source/State/StateManager.h
    Source/State/PresetManager.cpp
    Source/State/PresetManager.h

    # Utility
    Source/Utility/WavWriter.h
    Source/Utility/RandomUtils.cpp
    Source/Utility/RandomUtils.h
    Source/Utility/MathUtils.cpp
    Source/Utility/MathUtils.h

    # Parameters
    Source/Parameters/Parameters.cpp
    Source/Parameters/Parameters.h
)

# ----------------------------------
# 8) Link JUCE modules explicitly
# ----------------------------------
target_link_libraries(MagicSynth PRIVATE
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_audio_basics
    juce::juce_audio_utils
    juce::juce_dsp
)

# ----------------------------------
# 9) Disable unused features
# ----------------------------------
target_compile_definitions(MagicSynth PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
