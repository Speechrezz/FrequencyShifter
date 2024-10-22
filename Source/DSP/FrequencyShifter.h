/*
  ==============================================================================

    FrequencyShifter.h
    Created: 21 Oct 2024 8:56pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ComplexBuffer.h"
#include "HilbertProcessor.h"

namespace xynth
{
class FrequencyShifter
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept;

private:
    HilbertProcessor hilbertProcessor;

    ComplexBuffer<float> complexBuffer;

};
}