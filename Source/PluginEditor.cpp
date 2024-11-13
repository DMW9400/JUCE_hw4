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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

Hw4AudioProcessorEditor::~Hw4AudioProcessorEditor()
{
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
