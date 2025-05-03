#include "Plane.h"

Plane::Plane(Type planeType) : type(planeType)
{
    for (int i = 0; i < 5; ++i)
    {
        auto* node = new Node(i);
        addAndMakeVisible(node);
        nodes.add(node);
    }
}

void Plane::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black.withAlpha(0.8f));
    g.setColour(juce::Colours::lightblue);

    for (auto& conn : connections)
    {
        auto* a = nodes[conn.first];
        auto* b = nodes[conn.second];
        g.drawLine(a->getBounds().getCentreX(), a->getBounds().getCentreY(),
                   b->getBounds().getCentreX(), b->getBounds().getCentreY(), 2.0f);
    }
}

void Plane::resized()
{
    for (auto* node : nodes)
    {
        int x = juce::Random::getSystemRandom().nextInt(getWidth() - 20);
        int y = juce::Random::getSystemRandom().nextInt(getHeight() - 20);
        node->setTopLeftPosition(x, y);
    }
}

void Plane::mouseDown(const juce::MouseEvent& event)
{
    draggingNode = nullptr;
    for (auto* node : nodes)
        if (node->getBounds().contains(event.getPosition()))
            draggingNode = node;
}

void Plane::mouseDrag(const juce::MouseEvent& event)
{
    if (draggingNode != nullptr)
    {
        draggingNode->setCentrePosition(event.getPosition());

        for (auto* other : nodes)
        {
            if (other != draggingNode && other->getBounds().contains(event.getPosition()))
                tryCreateConnection(draggingNode, other);
        }
    }
}

void Plane::tryCreateConnection(Node* a, Node* b)
{
    auto pair = std::make_pair(a->getId(), b->getId());
    auto rev  = std::make_pair(b->getId(), a->getId());

    if (!connections.contains(pair) && !connections.contains(rev))
    {
        connections.add(pair);
        repaint();
    }
}

Plane::Type Plane::getType() const
{
    return type;
}

const juce::Array<Node*>& Plane::getNodes() const
{
    return nodes;
}

const juce::Array<std::pair<int, int>>& Plane::getConnections() const
{
    return connections;
}

// —— New methods for preset save/load ——

void Plane::clear()
{
    // Remove existing node components
    for (auto* node : nodes)
        removeChildComponent(node);
    nodes.clear();
    connections.clear();

    // Recreate default nodes
    resized();
}

void Plane::addConnection(int fromId, int toId)
{
    auto pair = std::make_pair(fromId, toId);
    auto rev  = std::make_pair(toId, fromId);

    if (!connections.contains(pair) && !connections.contains(rev))
    {
        connections.add(pair);
        repaint();
    }
}
