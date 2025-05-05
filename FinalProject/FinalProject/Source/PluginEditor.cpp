#include "PluginEditor.h"
#include "PluginProcessor.h"

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
        constexpr float thresh = 10.0f;
        int removeIdx = -1;

        for (int i = 0; i < (int)lineSegments.size(); ++i)
        {
            auto& s = lineSegments[i];
            juce::Point<float> nearest;
            float d = juce::Line<float>(predefinedDots[s.first],
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
    // 1) Clear all effect flags
    processor.setReverbEnabled     (false);
    processor.setDelayEnabled      (false);
    processor.setLPFEnabled        (false);
    processor.setHPFEnabled        (false);
    processor.setDistortionEnabled (false);

    // 2) Gather edges into a quick lookup with a small struct that has operator==
    struct Edge
    {
        int a, b;
        bool operator== (const Edge& other) const noexcept
        {
            return a == other.a && b == other.b;
        }
    };

    juce::Array<Edge> edges;
    for (auto& s : lineSegments)
        edges.add ({ (int)s.first, (int)s.second });

    auto hasEdge = [&edges](int x, int y)
    {
        return edges.contains ({ x, y }) || edges.contains ({ y, x });
    };

    // 3) Apply interior‐connection → effect rules
    if (hasEdge(0,3)) processor.setReverbEnabled     (true);  // top→bottom left
    if (hasEdge(1,4)) processor.setLPFEnabled        (true);  // top right→top left
    if (hasEdge(1,3)) processor.setHPFEnabled        (true);  // top right→bottom left
    if (hasEdge(4,2)) processor.setDistortionEnabled (true);  // top left→bottom right

    // 4) Count only the five “outer” edges:
    //    (0–1),(1–2),(2–3),(3–4),(4–0)
    int outerCount = 0;
    const std::pair<int,int> outers[] = {{0,1},{1,2},{2,3},{3,4},{4,0}};
    for (auto& o : outers)
        if (hasEdge(o.first, o.second))
            ++outerCount;

    // 5) Map outerCount→waveType:
    //    0→none(0), 1→sine(1), 2→tri(2), 3→rect10(3), 4→rect50(4), 5→saw(5)
    int waveType = 0;
    switch (outerCount)
    {
        case 0: waveType = 0; break;
        case 1: waveType = 1; break;
        case 2: waveType = 2; break;
        case 3: waveType = 3; break;
        case 4: waveType = 4; break;
        case 5: waveType = 5; break;
        default: waveType = 5; break;
    }

    // 6) Send to processor
    processor.setWaveType (waveType);
}
