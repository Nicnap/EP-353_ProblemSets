/*
  ==============================================================================
  
    DAFxTemplate
    This file is generated dynamically by CMake. VSCode may complain it can't find it until
    you generate the CMake build directory.
  
  ==============================================================================
*/
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <cmath>
#include <iostream>

// Function prototypes
int readSoundFile (juce::AudioBuffer<float>* buffer);
int writeSoundFile (juce::AudioBuffer<float> buffer, float sampleRate);
void process (juce::AudioBuffer<float> inBuffer, juce::AudioBuffer<float>* outBuffer, int keepBits);
short keep_bits_from_16 (short input, int keepBits);

//==============================================================================
int main ()
{
    // Read the source file into a buffer for processing
    juce::AudioBuffer<float> inBuffer;
    float sampleRate = readSoundFile(&inBuffer);
    if (!sampleRate)
        return 1;

    // Ask user how many bits to keep (range: 1 ~ 16)
    std::cout << "Enter the number of bits to keep (1-16): ";
    int keepBits = 0;
    std::cin >> keepBits;
    // Validate input
    if (keepBits < 1 || keepBits > 16)
    {
        std::cout << "Invalid input. The number of bits to keep must be between 1 and 16." << std::endl;
        return 1;
    }
    // Consume any leftover newline characters
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Create a buffer for processed audio data having the same size
    juce::AudioBuffer<float> outBuffer(inBuffer.getNumChannels(), inBuffer.getNumSamples());

    // Process the audio data with the bit quantization effect
    process(inBuffer, &outBuffer, keepBits);

    // Write the processed audio data to a file (AIFF format with 24-bit quality)
    if (writeSoundFile(outBuffer, sampleRate))
        return 1;

    std::cout << "Processing complete. Output file saved." << std::endl;
    return 0;
}

//------------------------------------------------------------------------------
// This function applies the bit reduction effect on each sample.
// It converts the float sample (in range -1.0f to 1.0f) to a 16-bit short,
// performs quantization using the keep_bits_from_16 function, and converts it
// back to float.
void process (juce::AudioBuffer<float> inBuffer, juce::AudioBuffer<float>* outBuffer, int keepBits)
{
    const int numChannels = inBuffer.getNumChannels();
    const int numSamples  = inBuffer.getNumSamples();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* readPtr  = inBuffer.getReadPointer(channel);
        float* writePtr = outBuffer->getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Convert float to a 16-bit integer sample.
            // Assuming the input float is normalized (i.e., -1.0f to 1.0f).
            short sampleShort = static_cast<short>(readPtr[sample] * 32768);

            // Apply the bit quantization/reduction effect.
            short processedShort = keep_bits_from_16(sampleShort, keepBits);

            // Convert back to float (normalizing the value)
            writePtr[sample] = static_cast<float>(processedShort) / 32768.0f;
        }
    }
}

//------------------------------------------------------------------------------
// Bit quantization/reduction function.
// This function reduces the effective number of bits in the 16-bit sample.
short keep_bits_from_16 (short input, int keepBits)
{
    // Calculate an offset to reduce quantization bias
    short prevent_offset = static_cast<unsigned short>(-1) >> (keepBits + 1);

    // Mask out the lower bits that are not kept.
    input &= (-1 << (16 - keepBits));

    // Add a small offset so that the resulting value is centered.
    return input + prevent_offset;
}

//------------------------------------------------------------------------------
// Reads a sound file (e.g. sine.wav) into the AudioBuffer.
// Returns the sample rate if successful; otherwise, returns 0.
int readSoundFile (juce::AudioBuffer<float>* buffer)
{
    std::cout << "Enter the path to the wave file to process (e.g. sine.wav): ";
    std::string filePath;
    std::getline(std::cin, filePath);
    juce::File file(filePath);

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    if (reader)
    {
        buffer->setSize(reader->numChannels, static_cast<int>(reader->lengthInSamples));

        // Read the whole file into the buffer.
        bool success = reader->read(buffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
        if (success)
            return reader->sampleRate;

        std::cout << "Failed to copy samples into the buffer from the source wave file." << std::endl;
        return 0;
    }

    std::cout << "Failed to read the source wave file." << std::endl;
    return 0;
}

//------------------------------------------------------------------------------
// Writes the AudioBuffer to a sound file using AIFF format with 24-bit quality.
// Returns 0 if successful, otherwise returns 1.
int writeSoundFile (juce::AudioBuffer<float> buffer, float sampleRate)
{
    std::cout << "Enter the path to save the AIFF file (e.g. reduct.aiff): ";
    std::string filePath;
    std::getline(std::cin, filePath);
    juce::File file(filePath);

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    // Use AIFF format writer for output. Setting bits per sample to 24.
    juce::AudioFormat* aiffFormat = formatManager.findFormatForFileExtension("aiff");
    if (!aiffFormat)
    {
        std::cout << "AIFF format not available." << std::endl;
        return 1;
    }

    std::unique_ptr<juce::FileOutputStream> fileStream(file.createOutputStream());
    if (!fileStream)
    {
        std::cout << "Failed to create output stream for the destination file." << std::endl;
        return 1;
    }

    std::unique_ptr<juce::AudioFormatWriter> writer(aiffFormat->createWriterFor(fileStream.release(),
                                                                                 sampleRate,
                                                                                 buffer.getNumChannels(),
                                                                                 24,
                                                                                 {},
                                                                                 0));

    if (writer)
    {
        bool success = writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        if (success)
            return 0;

        std::cout << "Failed to write samples to the destination file." << std::endl;
        return 1;
    }

    std::cout << "Failed to create a writer for the destination file." << std::endl;
    return 1;
}
