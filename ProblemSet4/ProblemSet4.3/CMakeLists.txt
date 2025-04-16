cmake_minimum_required(VERSION 3.15)

project(Problem4_3)

set(CMAKE_CXX_STANDARD 17)

set(JUCE_DIR "/Users/nknapp/Documents/GitHub/EP-353_ProblemSets/ProblemSet4/ProblemSet4.3/JUCE")

add_subdirectory(${JUCE_DIR} juce)

add_executable(Problem4_3
    Source/Main.cpp
)

target_link_libraries(Problem4_3
    PRIVATE
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_audio_formats
)
 