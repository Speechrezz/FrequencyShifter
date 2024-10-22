/*
  ==============================================================================

    ComplexRingMod.h
    Created: 22 Oct 2024 9:35am
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ComplexBuffer.h"

namespace xynth
{
class ComplexRingMod
{
public:
    ComplexRingMod(std::atomic<float>& frequencyParameter);

    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    void process(const ComplexBlock<float>& input, juce::dsp::AudioBlock<float>& output) noexcept;

private:
    std::atomic<float>& frequencyParameter;
    float radiansCoefficient = 0.f;
    float phase = 0.f;

};
}