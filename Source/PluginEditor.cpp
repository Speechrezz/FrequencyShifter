/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

FrequencyShifterAudioProcessorEditor::FrequencyShifterAudioProcessorEditor (FrequencyShifterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor(p), shiftAttach(*p.treeState.getParameter("shift"), shiftSlider)
{
    setSize(600, 400);

    addAndMakeVisible(shiftSlider);
    shiftSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    shiftSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
}

FrequencyShifterAudioProcessorEditor::~FrequencyShifterAudioProcessorEditor()
{}

void FrequencyShifterAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void FrequencyShifterAudioProcessorEditor::resized()
{
    auto shiftBounds = getLocalBounds().withSizeKeepingCentre(100, 100);
    shiftSlider.setBounds(shiftBounds);
}