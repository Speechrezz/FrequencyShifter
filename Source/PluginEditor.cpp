/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

FrequencyShifterAudioProcessorEditor::FrequencyShifterAudioProcessorEditor (FrequencyShifterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor(p)
{
    setSize(600, 400);
}

FrequencyShifterAudioProcessorEditor::~FrequencyShifterAudioProcessorEditor()
{}

void FrequencyShifterAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void FrequencyShifterAudioProcessorEditor::resized()
{
}