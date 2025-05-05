#include "PluginEditor.h"
#include "PluginProcessor.h"

WizardWaveAudioProcessorEditor::WizardWaveAudioProcessorEditor (WizardWaveAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    predefinedDots.resize (5);
    setSize (600, 450);

    // ADSR toggle
    addAndMakeVisible (adsrButton);
    adsrButton.setTopLeftPosition (10, 10);
    adsrButton.onClick = [this]()
    {
        auto* param = processor.getUseADSRParam();
        param->beginChangeGesture();
        param->setValueNotifyingHost (1.0f - param->get());
        param->endChangeGesture();
    };

    // Intensity slider
    intensitySlider.setRange (0.0, 100.0, 1.0);
    intensitySlider.setTextValueSuffix (" Intensity");
    intensitySlider.onValueChange = [this]()
    {
        processor.setIntensity ((float) intensitySlider.getValue());
    };
    addAndMakeVisible (intensitySlider);
}

WizardWaveAudioProcessorEditor::~WizardWaveAudioProcessorEditor() = default;

void WizardWaveAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF800080));
    g.setColour (juce::Colours::white);

    for (auto& pt : predefinedDots)
        g.fillEllipse (pt.x - 5.0f, pt.y - 5.0f, 10.0f, 10.0f);

    for (auto& seg : lineSegments)
    {
        auto& a = predefinedDots[seg.first];
        auto& b = predefinedDots[seg.second];
        g.drawLine (a.x, a.y, b.x, b.y, 2.0f);
    }
}

void WizardWaveAudioProcessorEditor::resized()
{
    adsrButton.setSize (100, 30);
    intensitySlider.setBounds (10, getHeight() - 40, getWidth() - 20, 30);

    float cx     = getWidth() * 0.5f;
    float cy     = (getHeight() - 50) * 0.5f + 20;
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
        constexpr float thresh = 10.0f;
        int removeIdx = -1;

        for (int i = 0; i < (int)lineSegments.size(); ++i)
        {
            auto& s       = lineSegments[i];
            juce::Point<float> nearest;
            float d       = juce::Line<float>(predefinedDots[s.first],
                                              predefinedDots[s.second])
                              .getDistanceFromPoint (e.position, nearest);
            if (d < thresh) { removeIdx = i; break; }
        }

        if (removeIdx >= 0)
            lineSegments.erase (lineSegments.begin() + removeIdx);

        processLineSegments();
        repaint();
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
    processor.setReverbEnabled     (false);
    processor.setDelayEnabled      (false);
    processor.setLPFEnabled        (false);
    processor.setHPFEnabled        (false);
    processor.setDistortionEnabled (false);

    struct Edge
    {
        int a, b;
        bool operator== (const Edge& o) const noexcept { return a==o.a && b==o.b; }
    };
    juce::Array<Edge> edges;
    for (auto& s : lineSegments) edges.add ({ (int)s.first, (int)s.second });
    auto hasEdge = [&edges](int x,int y){ return edges.contains({x,y})||edges.contains({y,x}); };

    // Interior effects
    if (hasEdge(0,3)) processor.setReverbEnabled     (true);
    if (hasEdge(1,4)) processor.setLPFEnabled        (true);
    if (hasEdge(1,3)) processor.setHPFEnabled        (true);
    if (hasEdge(4,2)) processor.setDistortionEnabled (true);

    // Outer edges count
    int outerCount = 0;
    const std::pair<int,int> outers[] = {{0,1},{1,2},{2,3},{3,4},{4,0}};
    for (auto& o : outers) if (hasEdge(o.first,o.second)) ++outerCount;

    int waveType = 0;
    switch (outerCount)
    {
        case 1: waveType = 1; break;
        case 2: waveType = 2; break;
        case 3: waveType = 3; break;
        case 4: waveType = 4; break;
        case 5: waveType = 5; break;
        default: waveType = 0; break;
    }

    processor.setWaveType (waveType);
}
