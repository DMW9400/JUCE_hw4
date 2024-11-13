/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MIDISynth.h"

//==============================================================================
/**
*/
class Hw4AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Hw4AudioProcessorEditor (Hw4AudioProcessor&);
    ~Hw4AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Hw4AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Hw4AudioProcessorEditor)
};
