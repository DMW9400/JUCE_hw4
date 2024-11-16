/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Hw4AudioProcessorEditor::Hw4AudioProcessorEditor (Hw4AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Initialize master gain slider
    masterGainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    masterGainSlider.setRange(0.0001f, 0.001f, 0.00001f); // Increment by 0.00001 for fine control
    masterGainSlider.setValue(0.0001f); // Default value matching ToneBank
    masterGainSlider.setTextValueSuffix(" Master Gain");
    masterGainSlider.setNumDecimalPlacesToDisplay(6); // Display more decimal places
    masterGainSlider.setSkewFactorFromMidPoint(0.00055f); // Optional: improves usability
    masterGainSlider.addListener(this);

    // Optional: Format the displayed text
    masterGainSlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 6);
    };

    addAndMakeVisible(masterGainSlider);

    // Initialize master gain label
    masterGainLabel.setText("Master Gain", juce::dontSendNotification);
    masterGainLabel.attachToComponent(&masterGainSlider, true);
    addAndMakeVisible(masterGainLabel);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

Hw4AudioProcessorEditor::~Hw4AudioProcessorEditor()
{
    masterGainSlider.removeListener(this);
}

void Hw4AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &masterGainSlider)
    {
        float newMasterGain = static_cast<float>(slider->getValue());
        audioProcessor.getToneBank().setMasterGain(newMasterGain);
    }
}


//==============================================================================
void Hw4AudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill background
       g.fillAll (juce::Colours::black);

       // Draw a simple message
       g.setColour (juce::Colours::white);
       g.setFont (15.0f);
       g.drawFittedText ("MIDI Synthesizer", getLocalBounds(), juce::Justification::centred, 1);
}

void Hw4AudioProcessorEditor::resized()
{
    masterGainSlider.setBounds(100, 100, 200, 20);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
