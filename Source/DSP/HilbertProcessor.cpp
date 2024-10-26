/*
  ==============================================================================

    HilbertProcessor.cpp
    Created: 26 Oct 2024 1:41pm
    Author:  Mark

  ==============================================================================
*/

#include "HilbertProcessor.h"

namespace xynth
{

void HilbertProcessor::prepare(const juce::dsp::ProcessSpec& spec, float passbandGain) noexcept
{
	HilbertIIRCoeffs coeffs;

	float freqFactor = std::min<float>(0.46f, 20000.f / float(spec.sampleRate));
	direct = coeffs.direct * 2.f * passbandGain * freqFactor;

	for (int i = 0; i < order; ++i) 
	{
		Complex coeff = coeffs.coeffs[i] * freqFactor * passbandGain;
		coeffsReal[i] = coeff.real();
		coeffsImag[i] = coeff.imag();

		Complex pole = std::exp(coeffs.poles[i] * freqFactor);
		polesReal[i] = pole.real();
		polesImag[i] = pole.imag();
	}

	states.resize(spec.numChannels);
	reset();
}

void HilbertProcessor::reset() noexcept
{
	for (auto& state : states) 
	{
		for (auto& value : state.real) 
			value = 0.f;
		for (auto& value : state.imag)
			value = 0.f;
	}
}

HilbertProcessor::Complex HilbertProcessor::processSample(float sample, int channel) noexcept
{
	jassert(channel < states.size());
	// Really we're just doing: state[i] = state[i]*poles[i] + sample*coeffs[i]
	// but std::complex is slow without -ffast-math, so we've unwrapped it

	State state = states[channel], newState;
	for (int i = 0; i < order; ++i) 
		newState.real[i] = state.real[i] * polesReal[i] - state.imag[i] * polesImag[i] + sample * coeffsReal[i];
	
	for (int i = 0; i < order; ++i) 
		newState.imag[i] = state.real[i] * polesImag[i] + state.imag[i] * polesReal[i] + sample * coeffsImag[i];
	
	states[channel] = newState;

	float resultReal = sample * direct;
	for (int i = 0; i < order; ++i) 
		resultReal += newState.real[i];
	
	float resultImag = 0;
	for (int i = 0; i < order; ++i) 
		resultImag += newState.imag[i];
	
	return { resultReal, resultImag };
}

HilbertProcessor::Complex HilbertProcessor::processSample(Complex sample, int channel) noexcept
{
	jassert(channel < states.size());
	// Really we're just doing: state[i] = state[i]*poles[i] + sample*coeffs[i]
	// but std::complex is slow without -ffast-math, so we've unwrapped it

	State state = states[channel], newState;
	for (int i = 0; i < order; ++i)
		newState.real[i] = state.real[i] * polesReal[i] - state.imag[i] * polesImag[i] 
						 + sample.real() * coeffsReal[i] - sample.imag() * coeffsImag[i];

	for (int i = 0; i < order; ++i)
		newState.imag[i] = state.real[i] * polesImag[i] + state.imag[i] * polesReal[i] 
						 + sample.real() * coeffsImag[i] + sample.imag() * coeffsReal[i];

	states[channel] = newState;

	float resultReal = sample.real() * direct;
	for (int i = 0; i < order; ++i)
		resultReal += newState.real[i];

	float resultImag = sample.imag() * direct;
	for (int i = 0; i < order; ++i)
		resultImag += newState.imag[i];

	return { resultReal, resultImag };
}

} // namespace xynth