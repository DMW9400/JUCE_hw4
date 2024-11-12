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
    enum WaveType {Sine, Squre, Sawtooth};
    
    Tone(float frequency, float velocity, WaveType waveType, double sampleRate);
    ~Tone();
    
    void setSampleRate(double newSampleRate);
    void setWaveType(WaveType newWaveType);
    void setFrequency(double newFrequency);
    void setGain (double newGain);
    void setReleased();
    void updateCounter();
    void updateTone();
    void processSample(float& sample);
    bool shouldBeRemoved();
    
private:
    WaveType waveType;
    double frequency;
    bool isReleased = false;
    double gain, velocity;
    long long counter;
    double sampleRate;
    
    float generateWaveSample();
    
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
    
private:
    std::vector<std::unique_ptr<Tone>> tones;
    Tone::WaveType wavetype;
    double sampleRate;
    double ATTACK_FACTOR, DECAY_FACTOR;
};

Tone::Tone(float frequency, float velocity, WaveType waveType, double sampleRate)
    :waveType(waveType),
    frequency(static_cast<double>(frequency)),
    isReleased(false),
    gain(0.01),
    velocity(static_cast<double>(velocity)),
    counter(0),
    sampleRate(sampleRate)
{
    
}
Tone::~Tone(){
    
}

void Tone::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
}

void Tone::setWaveType(WaveType newWaveType) {
    waveType = newWaveType;
}

void Tone::setFrequency(double newFrequency) {
    frequency = newFrequency;
}

void Tone::setGain(double newGain) {
    gain = newGain;
}

void Tone::setReleased() {
    isReleased = true;
}
