#pragma once
#include <JuceHeader.h>

class Node : public juce::Component
{
public:
    Node(int id);
    void paint(juce::Graphics& g) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    int getId() const;

private:
    int nodeId;
    juce::Point<float> dragStartPos;
};
