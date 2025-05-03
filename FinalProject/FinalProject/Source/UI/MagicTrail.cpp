#include "MagicTrail.h"

MagicTrail::MagicTrail() noexcept
{
    startTimerHz(60);    // Drive repaint at 60 Hz
}

void MagicTrail::addPoint(juce::Point<float> position)
{
    points.add(position);
    if (points.size() > maxPoints)
        points.remove(0);
}

void MagicTrail::paint(juce::Graphics& g)
{
    for (int i = 0; i < points.size(); ++i)
    {
        float alpha = float(i) / float(points.size());
        g.setColour(juce::Colours::cyan.withAlpha(alpha));
        auto& p = points.getReference(i);
        g.fillEllipse(p.x - 3.0f, p.y - 3.0f, 6.0f, 6.0f);
    }
}

void MagicTrail::timerCallback()
{
    repaint();
}
