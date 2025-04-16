#include <juce_core/juce_core.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_basics/juce_audio_basics.h>

int main()
{
    // Setup the AudioFormatManager
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats(); // Registers WAV, AIFF, etc. for flexibility

    // Define input and output
    juce::File inputFile = juce::File::getCurrentWorkingDirectory().getChildFile("SN.wav");
    juce::File outputFile = juce::File::getCurrentWorkingDirectory().getChildFile("ReverseSN.wav");

    if (!inputFile.existsAsFile())
    {
        std::cerr << "Error: SN.wav not found in current directory.\n";
        return 1;
    }

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(inputFile));

    if (reader == nullptr)
    {
        std::cerr << "Error: Could not read SN.wav.\n";
        return 1;
    }

    int numChannels = static_cast<int>(reader->numChannels);
    int numSamples = static_cast<int>(reader->lengthInSamples);
    juce::AudioBuffer<float> buffer(numChannels, numSamples);

    reader->read(&buffer, 0, numSamples, 0, true, true);

    // Reverse audio in each channel
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* data = buffer.getWritePointer(channel);
        std::reverse(data, data + numSamples);
    }

    // Prepare to write output
    juce::WavAudioFormat wavFormat;
    std::unique_ptr<juce::FileOutputStream> outputStream(outputFile.createOutputStream());

    if (outputStream == nullptr)
    {
        std::cerr << "Error: Could not open ReverseSN.wav for writing.\n";
        return 1;
    }

    std::unique_ptr<juce::AudioFormatWriter> writer(
        wavFormat.createWriterFor(outputStream.get(),
                                  reader->sampleRate,
                                  static_cast<unsigned int>(numChannels),
                                  16,
                                  {},
                                  0));

    if (writer == nullptr)
    {
        std::cerr << "Error: Could not create writer for ReverseSN.wav.\n";
        return 1;
    }

    outputStream.release(); // writer takes ownership

    if (!writer->writeFromAudioSampleBuffer(buffer, 0, numSamples))
    {
        std::cerr << "Error: Failed to write reversed audio.\n";
        return 1;
    }

    std::cout << "✅ Reversed audio successfully written to ReverseSN.wav\n";
    return 0;
}
