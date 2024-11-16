/*
  ==============================================================================

    MIDISynth.h
    Created: 11 Nov 2024 9:36:22pm
    Author:  David Matthew Welch

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Tone
{
public:
    enum WaveType {Sine, Square, Sawtooth};
    
    Tone(float frequency, float velocity, WaveType waveType, double sampleRate, double attackFactor, double decayFactor);
    ~Tone();
    
    void setSampleRate(double newSampleRate);
    void setWaveType(WaveType newWaveType);
    void setFrequency(double newFrequency);
    void setGain (double newGain);
    void setReleased();
    void updateTone();
    void processSample(float& sample);
    bool shouldBeRemoved() const;
    
    double getFrequency() const { return frequency; }

    
private:
    WaveType waveType;
    double frequency;
    bool isReleased = false;
    double gain, velocity;
    long long counter;
    double sampleRate;
    double attackFactor;
    double decayFactor;
    
    float generateWaveSample();
    void updateCounter();
    
};

class ToneBank 
{
public:
    ToneBank();
    ~ToneBank();
    
    void prepareToPlay(double newSampleRate);
    void setWaveType(Tone::WaveType waveType);
    void noteOn(float frequency, float velocity, Tone::WaveType wavetype);
    void noteOff(float frequency);
    void renderBuffer(juce::AudioBuffer<float>& buffer);
    
    Tone::WaveType getCurrentWaveType() const { return wavetype; }
    void setMasterGain(float newMasterGain) { masterGain = newMasterGain; }


    
private:
    std::vector<std::unique_ptr<Tone>> tones;
    Tone::WaveType wavetype;
    double sampleRate;
    double ATTACK_FACTOR, DECAY_FACTOR;
    
    float masterGain; // Master gain scaling factor
};

