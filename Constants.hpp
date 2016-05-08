#pragma once

// Use 96000 for sample rate for production
#define SAMPLE_RATE 48000
#define BUFFER_SIZE 1024
#define CHANNELS 2

// Info for wavetable generation
#define BASE_FREQUENCY 20.0
#define MAX_FREQUENCY 20000.0
#define MIN_FREQUENCY 20.0

// Reference information for MIDI. Both are A4
#define REFERENCE_FREQUENCY 440.0
#define REFERENCE_MIDI 69