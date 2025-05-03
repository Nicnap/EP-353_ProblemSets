#include "PresetManager.h"

PresetManager::PresetManager(PlaneManager& planes, StateManager& sm)
    : planeManager(planes), stateManager(sm)
{
}

void PresetManager::savePreset(const juce::File& file)
{
    juce::XmlElement presetXml("PRESET");

    // Save mode & randomness using the new getters
    presetXml.setAttribute("continuous", stateManager.isContinuousMode() ? 1 : 0);
    presetXml.setAttribute("randomness", stateManager.getRandomnessAmount());

    auto* planesXml = new juce::XmlElement("PLANES");
    for (auto* plane : planeManager.getPlanes())
    {
        juce::XmlElement* p = new juce::XmlElement("PLANE");
        p->setAttribute("type", (int)plane->getType());

        // Nodes
        for (auto* node : plane->getNodes())
        {
            auto c = node->getBounds().getCentre().toFloat();
            juce::XmlElement* ne = new juce::XmlElement("NODE");
            ne->setAttribute("id", node->getId());
            ne->setAttribute("x",  c.x);
            ne->setAttribute("y",  c.y);
            p->addChildElement(ne);
        }

        // Connections
        for (auto& conn : plane->getConnections())
        {
            juce::XmlElement* ce = new juce::XmlElement("CONNECTION");
            ce->setAttribute("from", conn.first);
            ce->setAttribute("to",   conn.second);
            p->addChildElement(ce);
        }

        planesXml->addChildElement(p);
    }
    presetXml.addChildElement(planesXml);

    file.deleteFile();
    file.replaceWithText(presetXml.toString());
}

void PresetManager::loadPreset(const juce::File& file)
{
    std::unique_ptr<juce::XmlElement> xml(juce::XmlDocument::parse(file));
    if (!xml || !xml->hasTagName("PRESET")) return;

    // Load mode & randomness via setters
    stateManager.setContinuousMode(xml->getBoolAttribute("continuous"));
    stateManager.setRandomnessAmount((float)xml->getDoubleAttribute("randomness"));

    auto* planesXml = xml->getChildByName("PLANES");
    if (!planesXml) return;

    for (auto* planeElem : planesXml->getChildIterator())
    {
        int t = planeElem->getIntAttribute("type");
        auto* plane = planeManager.getPlane((Plane::Type)t);
        if (!plane) continue;

        plane->clear();

        for (auto* child : planeElem->getChildIterator())
        {
            if (child->hasTagName("NODE"))
            {
                int id   = child->getIntAttribute("id");
                float x  = (float)child->getDoubleAttribute("x");
                float y  = (float)child->getDoubleAttribute("y");

                for (auto* node : plane->getNodes())
                    if (node->getId() == id)
                        node->setCentrePosition(x, y);
            }
            else if (child->hasTagName("CONNECTION"))
            {
                int from = child->getIntAttribute("from");
                int to   = child->getIntAttribute("to");
                plane->addConnection(from, to);
            }
        }

        plane->repaint();
    }
}
