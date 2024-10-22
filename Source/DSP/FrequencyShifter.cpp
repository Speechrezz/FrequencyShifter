/*
  ==============================================================================

    FrequencyShifter.cpp
    Created: 21 Oct 2024 8:56pm
    Author:  Mark

  ==============================================================================
*/

#include "FrequencyShifter.h"

namespace xynth
{

void FrequencyShifter::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    hilbertProcessor.prepare(spec);
    complexBuffer.setSize(spec.maximumBlockSize, spec.numChannels);
}

void FrequencyShifter::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    ComplexBlock<float> complexBlock(complexBuffer, 0, context.getInputBlock().getNumSamples());
    hilbertProcessor.process(context.getInputBlock(), complexBlock);
}

} // namespace xynth