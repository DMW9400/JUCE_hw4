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

// Process Sample
void Tone::processSample(float& sample) {
    // Generate the current wave sample and update the counter
    float waveSample = generateWaveSample();

    // Add the wave sample to the provided sample
    sample += waveSample;

    // Update the gain based on the envelope
    updateTone();
}

// Should Be Removed
bool Tone::shouldBeRemoved() const {
    return isReleased && (gain <= 0.0);
}

// Constructor Definition
ToneBank::ToneBank()
    : wavetype(Tone::Sine),    // Default wave type
      sampleRate(44100.0),     // Default sample rate
      ATTACK_FACTOR(1.05),     // Example value; adjust as needed
      DECAY_FACTOR(0.95)       // Example value; adjust as needed
{
    // Additional initialization if necessary
}

// Destructor Definition
ToneBank::~ToneBank() {
    // Unique pointers automatically clean up Tone instances
}

// Prepare to Play
void ToneBank::prepareToPlay(double newSampleRate) {
    sampleRate = newSampleRate;

    // Update sample rate for all active tones
    for (auto& tonePtr : tones) {
        tonePtr->setSampleRate(sampleRate);
    }
}

// Set Wave Type
void ToneBank::setWaveType(Tone::WaveType waveType) {
    wavetype = waveType;

    // As per problem set, do NOT affect tones currently playing
    // Hence, no iteration through existing tones
}

// Note On
void ToneBank::noteOn(float frequency, float velocity, Tone::WaveType waveType) {
    // Check polyphony limit (5 tones)
    if (tones.size() >= 5) {
        // Voice stealing strategy can be implemented here
        // For simplicity, we'll ignore the new note
        return;
    }

    // Check if the tone is already playing (based on frequency)
    bool toneAlreadyPlaying = false;
    for (const auto& tonePtr : tones) {
        if (std::abs(tonePtr->getFrequency() - frequency) < 0.1) { // Use getter
            toneAlreadyPlaying = true;
            break;
        }
    }

    // If tone is not already playing, create a new Tone instance
    if (!toneAlreadyPlaying) {
        std::unique_ptr<Tone> newTone = std::make_unique<Tone>(
            frequency,
            velocity,
            waveType,       // Use the waveType passed to noteOn
            sampleRate,
            ATTACK_FACTOR,
            DECAY_FACTOR
        );

        // Add the new Tone to the collection
        tones.emplace_back(std::move(newTone));
    }
}

// Note Off
void ToneBank::noteOff(float frequency) {
    for (auto& tonePtr : tones) {
        if (std::abs(tonePtr->getFrequency() - frequency) < 0.1) { // Use getter
            tonePtr->setReleased();
            break; // Assuming unique tones; remove if multiple instances can exist
        }
    }
}

// Render Buffer
void ToneBank::renderBuffer(juce::AudioBuffer<float>& buffer) {
    // Clear the buffer before rendering
    buffer.clear();

    // Iterate through each sample in the buffer
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        float mixedSample = 0.0f;

        // Iterate through all active tones
        for (auto it = tones.begin(); it != tones.end(); ) {
            // Process the current tone and add its sample to mixedSample
            it->get()->processSample(mixedSample);

            // Check if the tone should be removed
            if (it->get()->shouldBeRemoved()) {
                it = tones.erase(it); // Remove the tone from active voices
            } else {
                ++it; // Move to the next tone
            }
        }

        // Write the mixed sample to both left and right channels
        buffer.setSample(0, sample, mixedSample); // Left channel
        buffer.setSample(1, sample, mixedSample); // Right channel
    }
}
