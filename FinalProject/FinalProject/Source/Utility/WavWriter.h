#pragma once

#include <JuceHeader.h>

/**
 * A simple helper for writing raw audio samples to a .wav file.
 */
class WavWriter
{
public:
    static void writeToWavFile(const juce::File& file, const juce::AudioBuffer<float>& buffer, double sampleRate)
    {
        juce::WavAudioFormat format;
        std::unique_ptr<juce::FileOutputStream> stream(file.createOutputStream());

        if (stream.get() != nullptr)
        {
            std::unique_ptr<juce::AudioFormatWriter> writer(
                format.createWriterFor(stream.get(), sampleRate, buffer.getNumChannels(), 16, {}, 0));

            if (writer.get() != nullptr)
            {
                stream.release(); // (pass ownership to the writer)
                writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
            }
        }
    }
};
