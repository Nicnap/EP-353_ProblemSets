#pragma once
#include <JuceHeader.h>
#include "Node.h"

class Plane : public juce::Component
{
public:
    enum class Type { Waveform, Effects, Spatial };

    Plane(Type planeType);
    ~Plane() override { for (auto* n : nodes) delete n; }
    

    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;

    Type getType() const;
    const juce::Array<Node*>& getNodes() const;
    const juce::Array<std::pair<int, int>>& getConnections() const;

    // —— New methods for preset save/load ——  
    void clear();  
    void addConnection(int fromId, int toId);

private:
    Type type;
    juce::Array<Node*> nodes;
    juce::Array<std::pair<int, int>> connections;
    Node* draggingNode = nullptr;

    void tryCreateConnection(Node* a, Node* b);
};
