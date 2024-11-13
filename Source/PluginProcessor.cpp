/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Hw4AudioProcessor::Hw4AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Hw4AudioProcessor::~Hw4AudioProcessor()
{
}

//==============================================================================
const juce::String Hw4AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Hw4AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Hw4AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Hw4AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Hw4AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Hw4AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Hw4AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Hw4AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Hw4AudioProcessor::getProgramName (int index)
{
    return {};
}

void Hw4AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Hw4AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    toneBank.prepareToPlay(sampleRate);
}

void Hw4AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Hw4AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Hw4AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

       // Clear the buffer before rendering
       buffer.clear();

       for (const auto metadata : midiMessages)
       {
           const juce::MidiMessage& m = metadata.getMessage();
           if (m.isNoteOn())
           {
               float frequency = juce::MidiMessage::getMidiNoteInHertz(m.getNoteNumber());
               float velocity = m.getFloatVelocity() * 127.0f; // Ensure velocity is in 0-127 range

               // Check if the MIDI note is one of the special triggering notes
               // Example: Low C (48), D (50), E (52)
               if (m.getNoteNumber() == 48 || m.getNoteNumber() == 50 || m.getNoteNumber() == 52)
               {
                   // Set the wave type in ToneBank based on the special note
                   Tone::WaveType newWaveType = Tone::Sine;
                   if (m.getNoteNumber() == 48)
                       newWaveType = Tone::Sine;
                   else if (m.getNoteNumber() == 50)
                       newWaveType = Tone::Square;
                   else if (m.getNoteNumber() == 52)
                       newWaveType = Tone::Sawtooth;

                   toneBank.setWaveType(newWaveType);
               }
               else
               {
                   // Regular note-on event
                   toneBank.noteOn(frequency, velocity, toneBank.getCurrentWaveType());
               }
           }
           else if (m.isNoteOff())
           {
               float frequency = juce::MidiMessage::getMidiNoteInHertz(m.getNoteNumber());
               toneBank.noteOff(frequency);
           }
       }

       // Render the audio buffer from ToneBank
       toneBank.renderBuffer(buffer);
}

//==============================================================================
bool Hw4AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Hw4AudioProcessor::createEditor()
{
    return new Hw4AudioProcessorEditor (*this);
}

//==============================================================================
void Hw4AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Hw4AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Hw4AudioProcessor();
}
