#include "PluginEditor.h"
#include "SynthVoice.h"

WizardWaveAudioProcessorEditor::WizardWaveAudioProcessorEditor (WizardWaveAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    predefinedDots.resize (5);
    setSize (600, 400);

    addAndMakeVisible (adsrButton);
    adsrButton.setTopLeftPosition (10, 10);
    adsrButton.onClick = [this]()
    {
        auto* param = processor.getUseADSRParam();
        param->beginChangeGesture();
        param->setValueNotifyingHost (1.0f - param->get());
        param->endChangeGesture();
    };
}

WizardWaveAudioProcessorEditor::~WizardWaveAudioProcessorEditor() = default;

void WizardWaveAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF800080));
    g.setColour (juce::Colours::white);

    for (auto& pt : predefinedDots)
        g.fillEllipse (pt.x - 5.0f, pt.y - 5.0f, 10.0f, 10.0f);

    for (auto& segment : lineSegments)
    {
        auto& p1 = predefinedDots[segment.first];
        auto& p2 = predefinedDots[segment.second];
        g.drawLine (juce::Line<float>(p1, p2), 2.0f);
    }
}

void WizardWaveAudioProcessorEditor::resized()
{
    adsrButton.setSize (100, 30);

    float cx     = getWidth()  * 0.5f;
    float cy     = getHeight() * 0.5f;
    float radius = juce::jmin (cx, cy) * 0.8f;

    for (int i = 0; i < 5; ++i)
    {
        float angle = juce::MathConstants<float>::twoPi * i / 5.0f
                      - juce::MathConstants<float>::halfPi;
        predefinedDots[i].x = cx + std::cos (angle) * radius;
        predefinedDots[i].y = cy + std::sin (angle) * radius;
    }
}

void WizardWaveAudioProcessorEditor::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        constexpr float threshold = 10.0f;
        int indexToRemove = -1;

        for (int i = 0; i < (int)lineSegments.size(); ++i)
        {
            auto& seg = lineSegments[i];
            auto& p1  = predefinedDots[seg.first];
            auto& p2  = predefinedDots[seg.second];

            juce::Point<float> nearest;
            float dist = juce::Line<float>(p1, p2)
                             .getDistanceFromPoint (e.position, nearest);
            if (dist < threshold)
            {
                indexToRemove = i;
                break;
            }
        }

        if (indexToRemove >= 0)
        {
            lineSegments.erase (lineSegments.begin() + indexToRemove);
            processLineSegments();
            repaint();
        }
        return;
    }

    for (size_t i = 0; i < predefinedDots.size(); ++i)
    {
        if (e.position.getDistanceFrom (predefinedDots[i]) <= 10.0f)
        {
            selectedDotIndices.push_back ((int)i);
            break;
        }
    }

    if (selectedDotIndices.size() == 2)
    {
        lineSegments.emplace_back (selectedDotIndices[0],
                                   selectedDotIndices[1]);
        processLineSegments();
        selectedDotIndices.clear();
        repaint();
    }
}

void WizardWaveAudioProcessorEditor::processLineSegments()
{
    // Map 0→saw, 1→sine, 2→triangle, 3→rect10, 4→rect50, >=5→saw
    int count = (int) lineSegments.size();
    int waveType = 5; // default saw

    switch (count)
    {
        case 0: waveType = 5; break;
        case 1: waveType = 1; break;
        case 2: waveType = 2; break;
        case 3: waveType = 3; break;
        case 4: waveType = 4; break;
        default: waveType = 5; break;
    }

    if (auto* voice = dynamic_cast<SynthVoice*> (
                        processor.getSynth().getVoice (0)))
    {
        voice->setWaveType (waveType);
    }
}
