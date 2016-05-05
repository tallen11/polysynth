#include <iostream>
#include <PortAudio.h>
#include <curses.h>
#include <cmath>

#include "Constants.hpp"
#include "Oscillator.hpp"

static int portAudioCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	auto out = static_cast<float*>(output);
	auto oscillator = static_cast<Oscillator*>(userData);
	for (int i = 0; i < frameCount; ++i) {
		float sample = oscillator->getNextSample();
		*out++ = sample;
		*out++ = sample;

		oscillator->setFrequencyValue(oscillator->getFrequencyValue() + 0.0000001);
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
	auto oscillator = new Oscillator();
	oscillator->setFrequencyValue(0.0f);

	PaStream *stream = nullptr;
	setupPortAudio(stream, oscillator);

	char input;
	while ((input = getch()) != 'q');

	delete oscillator;

	return 0;
}