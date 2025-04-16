#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <cmath>
#include <iostream>

int main()
{
    // Open the input file.
    juce::File inputFile("sine.wav");
    if (!inputFile.existsAsFile())
    {
        std::cout << "Error: sine.wav not found!" << std::endl;
        return -1;
    }
    
    // Create an AudioFormatReader.
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(inputFile));
    if (!reader)
    {
        std::cout << "Error: Could not create reader for sine.wav" << std::endl;
        return -1;
    }
    
    // Read the input file into an audio buffer.
    int numChannels = static_cast<int>(reader->numChannels);
    int numSamples = static_cast<int>(reader->lengthInSamples);
    juce::AudioBuffer<float> buffer(numChannels, numSamples);
    reader->read(&buffer, 0, numSamples, 0, true, true);
    
    // Calculate sample counts for attack and decay portions.
    int attackSamples = static_cast<int>(numSamples * 0.1);
    int decaySamples = numSamples - attackSamples;
    
    // Apply the AD envelope to every sample in each channel.
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* samples = buffer.getWritePointer(channel);
        for (int i = 0; i < numSamples; ++i)
        {
            float envelope = 0.0f;
            if (i < attackSamples)
            {
                // Attack: linear interpolation from 0 to 1.
                envelope = static_cast<float>(i) / attackSamples;
            }
            else
            {
                // Decay: linear interpolation from 1 to 0.
                envelope = 1.0f - static_cast<float>(i - attackSamples) / decaySamples;
            }
            samples[i] *= envelope;
        }
    }
    
    // Write the enveloped buffer to "enveloped.wav".
    juce::File outputFile("enveloped.wav");
    outputFile.deleteFile();
    juce::WavAudioFormat wavFormat;
    std::unique_ptr<juce::FileOutputStream> outStream(outputFile.createOutputStream());
    std::unique_ptr<juce::AudioFormatWriter> writer(wavFormat.createWriterFor(outStream.get(),
                                                                              reader->sampleRate,
                                                                              numChannels,
                                                                              reader->bitsPerSample,
                                                                              {},
                                                                              0));
    if (writer)
    {
        writer->writeFromAudioSampleBuffer(buffer, 0, numSamples);
        outStream.release();
        std::cout << "enveloped.wav has been created successfully." << std::endl;
    }
    else
    {
        std::cout << "Error: Could not create writer for enveloped.wav" << std::endl;
        return -1;
    }
    
    return 0;
}
