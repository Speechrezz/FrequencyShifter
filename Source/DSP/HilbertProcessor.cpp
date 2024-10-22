/*
  ==============================================================================

    HilbertProcessor.cpp
    Created: 21 Oct 2024 8:56pm
    Author:  Mark

  ==============================================================================
*/

#include "HilbertProcessor.h"

namespace xynth
{

void HilbertProcessor::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    hilbertIIR = std::make_unique<HilbertIIR>(spec.sampleRate, spec.numChannels);
}

void HilbertProcessor::process(const juce::dsp::AudioBlock<const float>& input, ComplexBlock<float>& output) noexcept
{
    jassert(input.getNumSamples() == output.getNumSamples());

    for (int channel = 0; channel < input.getNumChannels(); ++channel)
    {
        const auto* inputPointer = input.getChannelPointer(channel);
        auto* outputPointer = output.getChannelPointer(channel);

        for (int i = 0; i < input.getNumSamples(); ++i)
            outputPointer[i] = (*hilbertIIR)(inputPointer[i], channel);
    }
}

} // namespace xynth