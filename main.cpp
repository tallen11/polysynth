#include <iostream>
#include <PortAudio.h>
#include <curses.h>
#include <cmath>

#include "Constants.hpp"
#include "WaveTables/WaveTable.hpp"

static int portAudioCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	auto out = static_cast<float*>(output);
	auto waveTable = static_cast<WaveTable*>(userData);

	for (int i = 0; i < frameCount; ++i) {
		float sample = waveTable->getNextSample();
		*out++ = sample;
		*out++ = sample;
	}

	return paContinue;
}

void setupPortAudio(PaStream *stream)
{
	PaError error;

	error = Pa_Initialize();
    if (error != paNoError) {
        std::cout << "PortAudio error: " << Pa_GetErrorText(error) << std::endl;
    }

    auto table = new WaveTable(wtSawtooth);
    
    error = Pa_OpenDefaultStream(&stream, 0, CHANNELS, paFloat32, SAMPLE_RATE, BUFFER_SIZE, portAudioCallback, table);
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
	PaStream *stream = nullptr;
	setupPortAudio(stream);

	std::cout << "Done!" << std::endl;

	while (true);

	return 0;
}