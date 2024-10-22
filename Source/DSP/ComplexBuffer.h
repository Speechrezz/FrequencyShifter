/*
  ==============================================================================

    ComplexBuffer.h
    Created: 21 Oct 2024 8:56pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace xynth
{

template<typename SampleType>
class ComplexBuffer
{
public:
    using Complex = std::complex<SampleType>;

    void setSize(int numSamples, int numChannels)
    {
        buffer.resize(numChannels);
        for (auto& channel : buffer)
            channel.resize(numSamples);
    }

    Complex* getChannelPointer(int channelIndex)
    {
        jassert(channelIndex < (int)buffer.size());
        return buffer[channelIndex].data();
    }

    const Complex* getChannelPointer(int channelIndex) const
    {
        jassert(channelIndex < (int)buffer.size());
        return buffer[channelIndex].data();
    }

    int getNumChannels() const { return (int)buffer.size(); }
    size_t getNumSamples() const { return buffer.empty() ? 0 : buffer[0].size(); }

protected:
    std::vector<std::vector<Complex>> buffer;

};

template<typename SampleType>
class ComplexBlock
{
public:
    using Buffer = ComplexBuffer<SampleType>;
    using Complex = std::complex<SampleType>;

    ComplexBlock(Buffer& newBuffer) 
        : buffer(newBuffer), offset(0), length(buffer.getNumSamples())
    {}

    ComplexBlock(Buffer& newBuffer, size_t newOffset, size_t newLength)
        : buffer(newBuffer), offset(newOffset), length(newLength)
    {
        jassert(offset < buffer.getNumSamples());
        jassert(offset + length <= buffer.getNumSamples());
    }

    Complex* getChannelPointer(int channelIndex)
    {
        return buffer.getChannelPointer(channelIndex);
    }

    const Complex* getChannelPointer(int channelIndex) const
    {
        return buffer.getChannelPointer(channelIndex);
    }

    int getNumChannels() const { return buffer.getNumChannels(); }
    size_t getNumSamples() const { return buffer.getNumSamples(); }

protected:
    Buffer& buffer;
    size_t offset, length;

};

}