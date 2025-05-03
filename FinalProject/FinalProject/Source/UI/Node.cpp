#include "Node.h"

Node::Node(int id) : nodeId(id)
{
    setSize(20, 20);
}

void Node::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.fillEllipse(getLocalBounds().toFloat());
}

void Node::mouseDrag(const juce::MouseEvent& event)
{
    auto delta = event.position - dragStartPos;
    setTopLeftPosition(getX() + delta.x, getY() + delta.y);
}

int Node::getId() const
{
    return nodeId;
}
