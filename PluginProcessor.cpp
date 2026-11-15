
#include "PluginProcessor.h"
#include "PluginEditor.h"

MFProductionVocalEnhancerAudioProcessor::
MFProductionVocalEnhancerAudioProcessor()
    : AudioProcessor(
        BusesProperties()
            .withInput("Input", juce::AudioChannelSet::stereo(), true)
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
      ),
      parameters(*this, nullptr, "PARAMETERS",
                 createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
MFProductionVocalEnhancerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "input", "Input",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "warmth", "Warmth", 0.0f, 100.0f, 25.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "saturation", "Saturation", 0.0f, 100.0f, 20.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive", 0.0f, 100.0f, 15.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "presence", "Presence", 0.0f, 100.0f, 20.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "air", "Air", 0.0f, 100.0f, 15.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", 0.0f, 100.0f, 100.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "output", "Output",
        juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), 0.0f));

    return layout;
}

void MFProductionVocalEnhancerAudioProcessor::prepareToPlay(
    double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    envelope = 0.0f;
}

void MFProductionVocalEnhancerAudioProcessor::releaseResources()
{
}

void MFProductionVocalEnhancerAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    const int channels = getTotalNumInputChannels();
    const int samples = buffer.getNumSamples();

    for (int ch = channels; ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, samples);

    const float inputGain = juce::Decibels::decibelsToGain(
        parameters.getRawParameterValue("input")->load());

    const float warmth =
        parameters.getRawParameterValue("warmth")->load() / 100.0f;

    const float saturation =
        parameters.getRawParameterValue("saturation")->load() / 100.0f;

    const float drive =
        parameters.getRawParameterValue("drive")->load() / 100.0f;

    const float presence =
        parameters.getRawParameterValue("presence")->load() / 100.0f;

    const float air =
        parameters.getRawParameterValue("air")->load() / 100.0f;

    const float mix =
        parameters.getRawParameterValue("mix")->load() / 100.0f;

    const float outputGain = juce::Decibels::decibelsToGain(
        parameters.getRawParameterValue("output")->load());

    const float driveAmount = 1.0f + drive * 7.0f + saturation * 5.0f;
    const float normalizer = std::tanh(driveAmount);

    for (int ch = 0; ch < channels; ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        float previous = samples > 0 ? data[0] : 0.0f;

        for (int i = 0; i < samples; ++i)
        {
            const float dry = data[i];
            float x = dry * inputGain;

            const float high = x - previous;
            previous = x;

            // Analog-style saturation
            float wet = std::tanh(x * driveAmount) / normalizer;

            // Warmth and harmonic enhancement
            wet += (x * std::abs(x)) * warmth * 0.12f;

            // Presence and air enhancement
            wet += high * presence * 0.30f;
            wet += high * air * 0.18f;

            // Dry / Wet
            const float processed =
                dry * (1.0f - mix) + wet * mix;

            data[i] = processed * outputGain;

            data[i] = juce::jlimit(-1.5f, 1.5f, data[i]);
        }
    }
}

juce::AudioProcessorEditor*
MFProductionVocalEnhancerAudioProcessor::createEditor()
{
    return new MFProductionVocalEnhancerAudioProcessorEditor(*this);
}

bool MFProductionVocalEnhancerAudioProcessor::hasEditor() const
{
    return true;
}

const juce::String
MFProductionVocalEnhancerAudioProcessor::getName() const
{
    return "MF Production Vocal Enhancer";
}

bool MFProductionVocalEnhancerAudioProcessor::acceptsMidi() const
{
    return false;
}

bool MFProductionVocalEnhancerAudioProcessor::producesMidi() const
{
    return false;
}

bool MFProductionVocalEnhancerAudioProcessor::isMidiEffect() const
{
    return false;
}

double MFProductionVocalEnhancerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MFProductionVocalEnhancerAudioProcessor::getNumPrograms()
{
    return 3;
}

int MFProductionVocalEnhancerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MFProductionVocalEnhancerAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String
MFProductionVocalEnhancerAudioProcessor::getProgramName(int index)
{
    if (index == 0) return "Male Vocal";
    if (index == 1) return "Female Vocal";
    if (index == 2) return "Lead Vocal";
    return {};
}

void MFProductionVocalEnhancerAudioProcessor::changeProgramName(
    int index, const juce::String& newName)
{
}

void MFProductionVocalEnhancerAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MFProductionVocalEnhancerAudioProcessor::setStateInformation(
    const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(
        getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MFProductionVocalEnhancerAudioProcessor();
}
