#include "PluginEditor.h"

MFProductionVocalEnhancerAudioProcessorEditor::MFProductionVocalEnhancerAudioProcessorEditor (MFProductionVocalEnhancerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (760, 470);
    title.setText ("MF PRODUCTION", juce::dontSendNotification);
    title.setFont (juce::Font (juce::FontOptions (25.0f, juce::Font::bold)));
    title.setColour (juce::Label::textColourId, juce::Colours::white);
    title.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (title);
    subtitle.setText ("VOCAL ENHANCER  |  ANALOG CHARACTER PROCESSOR", juce::dontSendNotification);
    subtitle.setFont (juce::Font (juce::FontOptions (11.0f)));
    subtitle.setColour (juce::Label::textColourId, juce::Colour (0xffaeb4bb));
    subtitle.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (subtitle);

    setupKnob(input, inputLabel, "INPUT"); setupKnob(warmth, warmthLabel, "WARMTH");
    setupKnob(drive, driveLabel, "DRIVE"); setupKnob(presence, presenceLabel, "PRESENCE");
    setupKnob(air, airLabel, "AIR"); setupKnob(mix, mixLabel, "MIX"); setupKnob(output, outputLabel, "OUTPUT");
    const std::pair<juce::Slider*, const char*> map[] = {{&input,"input"},{&warmth,"warmth"},{&drive,"drive"},{&presence,"presence"},{&air,"air"},{&mix,"mix"},{&output,"output"}};
    for (auto item : map) attachments.push_back (std::make_unique<SliderAttachment> (processor.parameters, item.second, *item.first));
}

void MFProductionVocalEnhancerAudioProcessorEditor::setupKnob (juce::Slider& s, juce::Label& l, const juce::String& name)
{
    s.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 76, 20);
    s.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffc4c9ce));
    s.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff41474d));
    s.setColour (juce::Slider::thumbColourId, juce::Colour (0xfff2f3f4));
    s.setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
    s.setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff202428));
    s.setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff454b51));
    addAndMakeVisible (s);
    l.setText (name, juce::dontSendNotification);
    l.setFont (juce::Font (juce::FontOptions (11.0f, juce::Font::bold)));
    l.setColour (juce::Label::textColourId, juce::Colour (0xffd7dadd));
    l.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (l);
}

void MFProductionVocalEnhancerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff15191d));
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff30363c), 0, 0, juce::Colour (0xff171b1f), 0, (float)getHeight(), false));
    g.fillRoundedRectangle (10.0f, 10.0f, (float)getWidth()-20.0f, (float)getHeight()-20.0f, 14.0f);
    g.setColour (juce::Colour (0xff596169)); g.drawRoundedRectangle (10.0f, 10.0f, (float)getWidth()-20.0f, (float)getHeight()-20.0f, 14.0f, 1.0f);
    g.setColour (juce::Colour (0xff70777e)); g.drawLine (35.0f, 91.0f, (float)getWidth()-35.0f, 91.0f, 1.0f);
    g.setColour (juce::Colour (0xff0d1013)); g.fillRoundedRectangle (27.0f, 118.0f, (float)getWidth()-54.0f, 300.0f, 10.0f);
    g.setColour (juce::Colour (0xff3d444b)); g.drawRoundedRectangle (27.0f, 118.0f, (float)getWidth()-54.0f, 300.0f, 10.0f, 1.0f);
}

void MFProductionVocalEnhancerAudioProcessorEditor::resized()
{
    title.setBounds (25, 24, getWidth()-50, 34);
    subtitle.setBounds (25, 59, getWidth()-50, 20);
    const int top = 145, knobW = 150, knobH = 190;
    const int startX = 32;
    juce::Slider* sliders[] = {&input,&warmth,&drive,&presence,&air,&mix,&output};
    juce::Label* labels[] = {&inputLabel,&warmthLabel,&driveLabel,&presenceLabel,&airLabel,&mixLabel,&outputLabel};
    for (int i=0; i<7; ++i)
    {
        const int x = startX + i * 100;
        labels[i]->setBounds (x, top, 96, 22);
        sliders[i]->setBounds (x+2, top+24, 92, knobH-24);
    }
}
