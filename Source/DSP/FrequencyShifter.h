/*
  ==============================================================================

    FrequencyShifter.h
    Created: 21 Oct 2024 8:56pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Vendor/hilbert-iir/hilbert.h"
#include "FilterStack.h"

namespace xynth
{
class FrequencyShifter
{
public:
    FrequencyShifter(std::atomic<float>& frequencyParameter);

    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    void process(juce::dsp::ProcessContextReplacing<float>& context) noexcept;

private:
    using HilbertIIR = signalsmith::hilbert::HilbertIIR<float>;
    std::unique_ptr<HilbertIIR> hilbertIIR;

private:
    std::atomic<float>& frequencyParameter;
    float radiansCoefficient = 0.f;
    float phase = 0.f;

private:
    void updateFilters(float frequency);
    using FilterType = IIRFilterStack<4>;
    FilterType highpassFilter, lowpassFilter;
    double sampleRate = -1.0;
    float nyquistFrequency = -1.f;

};
}