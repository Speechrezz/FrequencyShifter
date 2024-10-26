/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FrequencyShifterAudioProcessor::FrequencyShifterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       treeState(*this, nullptr, "PARAMETER", createParameterLayout()),
       frequencyShifter(*treeState.getRawParameterValue("shift"))
#endif
{}

juce::AudioProcessorValueTreeState::ParameterLayout FrequencyShifterAudioProcessor::createParameterLayout()
{
    using namespace juce;
    AudioProcessorValueTreeState::ParameterLayout layout;

    // --Frequency Shift--
    layout.add(std::make_unique<AudioParameterFloat>(ParameterID{ "shift",  1 }, "Shift", -1000.f, 1000.f, 0.f));

    return layout;
}

FrequencyShifterAudioProcessor::~FrequencyShifterAudioProcessor()
{}

//==============================================================================
const juce::String FrequencyShifterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FrequencyShifterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FrequencyShifterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FrequencyShifterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FrequencyShifterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FrequencyShifterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FrequencyShifterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FrequencyShifterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FrequencyShifterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FrequencyShifterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FrequencyShifterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const auto channels = juce::jmax(getTotalNumInputChannels(), getTotalNumOutputChannels());
    juce::dsp::ProcessSpec spec{ sampleRate, (juce::uint32)samplesPerBlock, (juce::uint32)channels };
    DBG("prepareToPlay() - sampleRate: " << sampleRate << ", maxBlockSize: " << (int)spec.maximumBlockSize << ", numChannels: " << (int)spec.numChannels);

    frequencyShifter.prepare(spec);
}

void FrequencyShifterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void FrequencyShifterAudioProcessor::reset()
{
    frequencyShifter.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FrequencyShifterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FrequencyShifterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    frequencyShifter.process(context);
}

//==============================================================================
bool FrequencyShifterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FrequencyShifterAudioProcessor::createEditor()
{
    return new FrequencyShifterAudioProcessorEditor (*this);
}

//==============================================================================
void FrequencyShifterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    DBG("Save state...");
    auto xmlState = std::make_unique<juce::XmlElement>(JucePlugin_Name);
    xmlState->setAttribute("version", JucePlugin_VersionString);

    auto xmlTreeState = treeState.copyState().createXml();
    xmlState->addChildElement(xmlTreeState.release());

    DBG("\nXML:\n" << xmlState->toString());
    copyXmlToBinary(*xmlState, destData);
}

void FrequencyShifterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    DBG("Load state...");
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (!xmlState)
        return;

    DBG("\nXML:\n" << xmlState->toString());
    if (auto* xmlTreeState = xmlState->getChildByName(treeState.state.getType()))
        treeState.replaceState(juce::ValueTree::fromXml(*xmlTreeState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FrequencyShifterAudioProcessor();
}
