#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <cmath>
#include <iostream>

int main()
{
    constexpr double sampleRate = 44100.0;
    constexpr double duration = 2.0; // seconds
    constexpr int numSamples = static_cast<int>(sampleRate * duration);
    constexpr double frequency = 440.0; // Hz (A4)

    juce::AudioBuffer<float> buffer(1, numSamples);
    float* samples = buffer.getWritePointer(0);

    for (int n = 0; n < numSamples; ++n)
    {
        double t = static_cast<double>(n) / sampleRate;
        double value = 0.0;

        // Additive synthesis approximation of triangle wave
        for (int k = 0; k < 10; ++k) // 10 harmonics
        {
            int harmonic = 2 * k + 1;
            double sign = (k % 2 == 0) ? 1.0 : -1.0;
            value += sign * std::pow(-1, k) * std::sin(2.0 * juce::MathConstants<double>::pi * harmonic * frequency * t) / (harmonic * harmonic);
        }

        samples[n] = static_cast<float>((8.0 / (juce::MathConstants<double>::pi * juce::MathConstants<double>::pi)) * value);
    }

    // Output file
    juce::File outputFile = juce::File::getCurrentWorkingDirectory().getChildFile("triangle.wav");

    juce::WavAudioFormat wavFormat;
    std::unique_ptr<juce::FileOutputStream> outputStream(outputFile.createOutputStream());

    if (outputStream == nullptr)
    {
        std::cerr << "Failed to create output stream." << std::endl;
        return 1;
    }

    std::unique_ptr<juce::AudioFormatWriter> writer(
        wavFormat.createWriterFor(outputStream.get(), sampleRate, 1, 16, {}, 0)
    );

    if (writer == nullptr)
    {
        std::cerr << "Failed to create writer." << std::endl;
        return 1;
    }

    outputStream.release(); // writer owns the stream now
    writer->writeFromAudioSampleBuffer(buffer, 0, numSamples);

    std::cout << "Wrote triangle.wav!" << std::endl;
    return 0;
}
