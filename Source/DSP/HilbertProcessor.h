/*
  ==============================================================================

    HilbertProcessor.h
    Created: 26 Oct 2024 1:41pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Vendor/hilbert-iir/hilbert.h"

namespace xynth
{

// Reimplementation of Signalsmith's HilbertIIR class.
class HilbertProcessor
{
public:
    using Complex = std::complex<float>;
    using HilbertIIRCoeffs = signalsmith::hilbert::HilbertIIRCoeffs<float>;
    static constexpr int order = HilbertIIRCoeffs::order;

public:
    HilbertProcessor() = default;

    void prepare(const juce::dsp::ProcessSpec& spec, float passbandGain = 2.f) noexcept;
    void reset() noexcept;

    Complex processSample(float sample, int channel) noexcept;
    Complex processSample(Complex sample, int channel) noexcept;

protected:
    using Array = std::array<float, order>;
    struct State 
    {
        Array real, imag;
    };

    Array coeffsReal, coeffsImag, polesReal, polesImag;
    std::vector<State> states;
    float direct;

};
}