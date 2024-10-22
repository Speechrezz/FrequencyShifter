/*
  ==============================================================================

    HilbertProcessor.h
    Created: 21 Oct 2024 8:56pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ComplexBuffer.h"
#include "Vendor/hilbert-iir/hilbert.h"

namespace xynth
{
class HilbertProcessor
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    void process(const juce::dsp::AudioBlock<const float>& input, ComplexBlock<float>& output) noexcept;

private:
    using HilbertIIR = signalsmith::hilbert::HilbertIIR<float>;
    std::unique_ptr<HilbertIIR> hilbertIIR;

};
}