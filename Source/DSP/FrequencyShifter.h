/*
  ==============================================================================

    FrequencyShifter.h
    Created: 21 Oct 2024 8:56pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "HilbertProcessor.h"
#include "Vendor/hilbert-iir/hilbert.h"

namespace xynth
{
class FrequencyShifter
{
public:
    FrequencyShifter(std::atomic<float>& frequencyParameter);

    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    void process(juce::dsp::ProcessContextReplacing<float>& context) noexcept;

    void reset() noexcept;

private:
    using HilbertIIR = signalsmith::hilbert::HilbertIIR<float>;
    HilbertProcessor hilbertProcessor, antialiasingProcessor;

private:
    std::atomic<float>& frequencyParameter;
    float radiansCoefficient = 0.f;
    float phase = 0.f;

};
}