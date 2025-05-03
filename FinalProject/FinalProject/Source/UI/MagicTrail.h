#pragma once

#include <JuceHeader.h>

/** 
    A component that draws a fading trail of mouse positions.
    Uses JUCE’s Timer (inherited) and juce::Array for storage.
*/
class MagicTrail : public juce::Component,
                   private juce::Timer
{
public:
    MagicTrail() noexcept;                        // Marked noexcept to match override warning
    ~MagicTrail() override = default;

    /** Add a new point to the trail */
    void addPoint(juce::Point<float> position);

    /** Paint the trail */
    void paint(juce::Graphics& g) override;

private:
    /** Timer callback to drive repaints */
    void timerCallback() override;

    juce::Array<juce::Point<float>> points;      // Use JUCE Array, not std::vector
    static constexpr int maxPoints = 60;         // Maximum trail length
};
