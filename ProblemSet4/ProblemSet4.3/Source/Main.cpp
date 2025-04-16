#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <cmath>
#include <iostream>


// Define constant values as global variables for ease of access
const float kSampleRate = 44100.0f;
const float kSec = 1.0f;
const float kNumFrames = kSampleRate * kSec;
const int kNumChannels = 2;
const float kSineWaveFrequency = 250.0f;  // Sine wave frequency (Hz)
const float kLFOFrequency = 2.0f;        // LFO frequency (Hz)
const float kAmplitude = 0.25f;

int main (int argc, char* argv[])
{
    // Prompt for the output
    std::cout << "Enter the path for the destination .wav file: ";
    std::string destFilePath;
    std::getline(std::cin, destFilePath);

    // Create a File object from the path
    juce::File destFile(destFilePath);
    
    // Create an audio format manager and register the wav format
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    // Create a buffer to hold the samples
    juce::AudioBuffer<float> buffer(kNumChannels, kNumFrames);
    
    //Compute sine wave with tremolo
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float *channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Compute time in seconds
            float t = sample / kSampleRate;
            
            // Sine wave component (250Hz)
            float sineWave = kAmplitude * std::sin(2.0 * M_PI * kSineWaveFrequency * t);
            
            // LFO component (2Hz) with a unipolar output (0 to 1)
            float lfo = 0.5f * (std::sin(2.0 * M_PI * kLFOFrequency * t) + 1.0f);
            
            // Apply tremolo by multiplying sine wave by LFO signal
            channelData[sample] = sineWave * lfo;
        }
    }
    
    // Create a unique_ptr for the writer
    std::unique_ptr<juce::AudioFormatWriter> writer(formatManager.findFormatForFileExtension("wav")
                                          ->createWriterFor(new juce::FileOutputStream(destFile),
                                                             kSampleRate,
                                                             kNumChannels,
                                                             16,
                                                             {},
                                                             0));

    if (writer)
    {
       // Write the mixed samples to the output file
       writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }
    else
    {
       std::cout << "Failed to create a writer for the mixed file." << std::endl;
    }

    std::cout << "Tremolo effect applied, and file written to: " << destFile.getFullPathName() << std::endl;
    return 0;
}
