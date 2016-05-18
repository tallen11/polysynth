#pragma once

// Ugly way of changing sample rate. Fix later
static double synthSampleRate = 44100.0;

#define SAMPLE_RATE (synthSampleRate)
#define BUFFER_SIZE 512
#define CHANNELS 2

// Info for wavetable generation
#define BASE_FREQUENCY 20.0
#define MAX_FREQUENCY 20000.0
#define MIN_FREQUENCY 20.0

// Reference information for MIDI. Both are A4
#define REFERENCE_FREQUENCY 440.0
#define REFERENCE_MIDI 69