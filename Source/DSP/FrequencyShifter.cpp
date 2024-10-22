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

FrequencyShifter::FrequencyShifter(std::atomic<float>& f) : frequencyParameter(f)
{}

void FrequencyShifter::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    hilbertIIR = std::make_unique<HilbertIIR>(spec.sampleRate, spec.numChannels);
    radiansCoefficient = juce::MathConstants<float>::twoPi / (float)spec.sampleRate;
}

void FrequencyShifter::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    const auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();

    const float frequency = frequencyParameter.load(std::memory_order_relaxed);
    const float phaseDelta = frequency * radiansCoefficient;
    const float startPhase = phase;

    for (int channel = 0; channel < inputBlock.getNumChannels(); ++channel)
    {
        const auto* inputPointer = inputBlock.getChannelPointer(channel);
        auto* outputPointer = outputBlock.getChannelPointer(channel);
        phase = startPhase;

        for (int i = 0; i < inputBlock.getNumSamples(); ++i)
        {
            // Hilbert Filter
            const auto filteredSample = (*hilbertIIR)(inputPointer[i], channel);
            
            // Heterodyne/ringmod
            const auto phaser = std::polar(1.f, phase);
            outputPointer[i] = (filteredSample * phaser).real();
            phase += phaseDelta;
        }
    }

    phase = std::fmod(phase, juce::MathConstants<float>::twoPi);
}

} // namespace xynth