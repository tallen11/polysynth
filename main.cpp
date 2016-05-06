#include <iostream>
#include <PortAudio.h>
#include <cmath>

#include "Constants.hpp"
#include "Oscillator.hpp"
#include "Synth.hpp"
#include "Util.hpp"

static int count = 0;
static bool pressing = true;
static int portAudioCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	(void)input;
	(void)timeInfo;
	(void)statusFlags;

	auto out = static_cast<float*>(output);
	auto synth = static_cast<Synth*>(userData);
	for (int i = 0; i < frameCount; ++i) {
		if (count % SAMPLE_RATE == 0) {
			if (pressing) {
				synth->keyPressed(0);
			} else {
				synth->keyReleased(0);
			}

			pressing = !pressing;
		}

		float sample = static_cast<float>(synth->getNextSample());
		*out++ = sample;
		*out++ = sample;

		count++;
	}

	return paContinue;
}

void setupPortAudio(PaStream *stream, void *userData)
{
	PaError error;

	error = Pa_Initialize();
    if (error != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
    }
    
    error = Pa_OpenDefaultStream(&stream, 0, CHANNELS, paFloat32, SAMPLE_RATE, BUFFER_SIZE, portAudioCallback, userData);
    if (error != paNoError) {
        std::cout << "PortAudio stream error: " << Pa_GetErrorText(error) << std::endl;
    }
    
    error = Pa_StartStream(stream);
    if (error != paNoError) {
        std::cout << "PortAudio stream start error: " << Pa_GetErrorText(error) << std::endl;
    }
}

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	// auto oscillator = new Oscillator();
	// oscillator->setFrequencyValue(convertRanges(440.0, 20.0, 20000.0, 0.0, 1.0));

	auto synth = new Synth();

	PaStream *stream = nullptr;
	setupPortAudio(stream, synth);

	while (true);

	// delete oscillator;

	return 0;
}