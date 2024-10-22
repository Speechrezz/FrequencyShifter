/*
  ==============================================================================

    ComplexRingMod.cpp
    Created: 22 Oct 2024 9:35am
    Author:  Mark

  ==============================================================================
*/

#include "ComplexRingMod.h"

namespace xynth
{

ComplexRingMod::ComplexRingMod(std::atomic<float>& f)
    : frequencyParameter(f)
{}

void ComplexRingMod::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    radiansCoefficient = juce::MathConstants<float>::twoPi / (float)spec.sampleRate;
}

void ComplexRingMod::process(const ComplexBlock<float>& input, juce::dsp::AudioBlock<float>& output) noexcept
{
    jassert(input.getNumChannels() == output.getNumChannels());
    jassert(input.getNumSamples() == output.getNumSamples());

    const float frequency = frequencyParameter.load(std::memory_order_relaxed);
    const float phaseDelta = frequency * radiansCoefficient;
    const float startPhase = phase;

    for (int channel = 0; channel < output.getNumChannels(); ++channel)
    {
        phase = startPhase;
        const auto* inputPointer = input.getChannelPointer(channel);
        auto* outputPointer = output.getChannelPointer(channel);

        for (size_t i = 0; i < output.getNumSamples(); ++i)
        {
            const auto phaser = std::polar(1.f, phase);
            outputPointer[i] = (inputPointer[i] * phaser).real();

            phase += phaseDelta;
        }

    }

    phase = std::fmod(phase, juce::MathConstants<float>::twoPi);
}

} // namespace xynth