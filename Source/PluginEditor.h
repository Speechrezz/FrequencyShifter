/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class FrequencyShifterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    FrequencyShifterAudioProcessorEditor (FrequencyShifterAudioProcessor&);
    ~FrequencyShifterAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    FrequencyShifterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyShifterAudioProcessorEditor)
};
