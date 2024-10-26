/*
  ==============================================================================

    FilterStack.h
    Created: 17 Jun 2023 3:49:49pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace xynth
{

/// Usage:
///     // IIR Filter example
///     IIRFilterStack<2> filterStack;
///     using Coefficients = IIRFilterStack<>::State;
///     *filterStack.state = *Coefficients::makeNotch(sampleRate, frequency, filterQ);
/// 
///     // State Variable Filter example
///     StateVariableFilterStack<2> filterStack;
///     using FilterType = StateVariableFilterStack<>::State::Type;
///     filterStack.state->type = FilterType::lowPass;
///     filterStack.state->setCutOffFrequency(sampleRate, frequency);

/// <summary>
/// Stacks multiple identical filters, allowing for higher order filters.
/// View header file for usage examples.
/// </summary>
/// <typeparam name="Filter">Type of filter (IIR, StateVariable, etc.)</typeparam>
/// <typeparam name="FilterState">Type of filter state</typeparam>
/// <typeparam name="filterCount">Number of stacked filters</typeparam>
template<class Filter, typename FilterState, size_t filterCount = 2>
class FilterStack
{
public:
    FilterStack() : state(new FilterState)
    {
        for (auto& filter : filters)
            filter.state = state;
    }

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        for (auto& filter : filters)
            filter.prepare(spec);
    }

    void process(juce::dsp::ProcessContextReplacing<float>& context)
    {
        jassert(!filters.empty());

        for (auto& filter : filters)
            filter.process(context);
    }

    void process(juce::dsp::ProcessContextNonReplacing<float>& context)
    {
        jassert(!filters.empty());
        filters[0].process(context);

        juce::dsp::ProcessContextReplacing<float> onlyOutputContext(context.getOutputBlock());
        for (size_t i = 1; i < filters.size(); i++)
            filters[i].process(onlyOutputContext);
    }

    // Process using only a single filter
    template<class ContextType>
    void processSingle(ContextType& context, const size_t filterIndex)
    {
        jassert(!filters.empty());
        jassert(filterIndex < filterCount);

        filters[filterIndex].process(context);
    }

    void reset()
    {
        for (auto& filter : filters)
            filter.reset();
    }

    typename FilterState::Ptr state;

    using FilterType = Filter;
    using State = FilterState;

private:
    using FilterDuplicator = juce::dsp::ProcessorDuplicator<
        Filter, FilterState>;

    std::array<FilterDuplicator, filterCount> filters;

};

template<size_t filterCount = 2>
using IIRFilterStack = FilterStack<
    juce::dsp::IIR::Filter<float>,
    juce::dsp::IIR::Coefficients<float>,
    filterCount>;

template<size_t filterCount = 2>
using StateVariableFilterStack = FilterStack<
    juce::dsp::StateVariableFilter::Filter<float>,
    juce::dsp::StateVariableFilter::Parameters<float>,
    filterCount>;

} // namespace xynth