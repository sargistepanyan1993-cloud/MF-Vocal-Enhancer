
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MFProductionVocalEnhancerAudioProcessorEditor
    : public juce::AudioProcessorEditor
{
public:
    explicit MFProductionVocalEnhancerAudioProcessorEditor(
        MFProductionVocalEnhancerAudioProcessor& p);

    ~MFProductionVocalEnhancerAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment =
        juce::AudioProcessorValueTreeState::SliderAttachment;

    MFProductionVocalEnhancerAudioProcessor& processor;

    juce::Label title, subtitle;

    juce::Slider input, warmth, saturation, drive,
                 presence, air, mix, output;

    juce::Label inputLabel, warmthLabel, saturationLabel,
                driveLabel, presenceLabel, airLabel,
                mixLabel, outputLabel;

    std::vector<std::unique_ptr<SliderAttachment>> attachments;

    void setupKnob(juce::Slider&, juce::Label&,
                   const juce::String&);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        MFProductionVocalEnhancerAudioProcessorEditor)
};
