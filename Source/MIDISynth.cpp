/*
  ==============================================================================

    MIDISynth.cpp
    Created: 11 Nov 2024 9:36:22pm
    Author:  David Matthew Welch

  ==============================================================================
*/

#include "MIDISynth.h"

Tone::Tone(float frequency, float velocity, WaveType waveType, double sampleRate, double attackFactor, double decayFactor)
    :waveType(waveType),
    frequency(static_cast<double>(frequency)),
    isReleased(false),
    gain(0.01),
    velocity(static_cast<double>(velocity)),
    counter(0),
    sampleRate(sampleRate),
    attackFactor(attackFactor),
    decayFactor(decayFactor)
{
    velocity = std::clamp(velocity, 0.0f, 127.0f) / 127.0f;
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

void Tone::updateTone() {

    if (!isReleased) {
        // Attack Phase: Increase gain towards velocity
        gain *= attackFactor;

        // Clamp gain to not exceed the normalized velocity
        if (gain >= velocity) {
            gain = velocity;
        }
    } else {
        // Release Phase: Decrease gain
        gain *= decayFactor;

        // Optional: Clamp gain to not go below zero
        if (gain <= 0.0) {
            gain = 0.0;
        }
    }
}

void Tone::updateCounter() {
    // Increment the counter based on frequency and sample rate
    double phaseIncrement = frequency / sampleRate;
    counter += static_cast<long long>(phaseIncrement * 1000000); // Scaling factor for precision

    // Wrap the counter to prevent overflow
    if (counter > 1000000) {
        counter -= 1000000;
    }
}

float Tone::generateWaveSample() {
    float sample = 0.0f;

    // Calculate the phase as a value between 0 and 1
    double phase = static_cast<double>(counter) / 1000000.0; // Example normalization based on counter scaling

    switch (waveType) {
        case Sine:
            sample = static_cast<float>(std::sin(2.0 * M_PI * phase)) * static_cast<float>(gain);
            break;
        case Square:
            sample = (phase < 0.5) ? static_cast<float>(gain) : static_cast<float>(-gain);
            break;
        case Sawtooth:
            sample = static_cast<float>((2.0 * phase) - 1.0) * static_cast<float>(gain);
            break;
        default:
            sample = 0.0f;
            break;
    }

    // Update the counter after generating the sample
    updateCounter();

    return sample;
}
