#include <iostream>
#include <PortAudio.h>
#include <cmath>
#include <string>

#include "Constants.hpp"
#include "Oscillator.hpp"
#include "Synth.hpp"
#include "Util.hpp"

#include "Libraries/RtMidi/RtMidi.h"

static int count = 0;
static bool pressing = true;
static int key = 0;
static int inc = 1;
static int notes[] = {60, 62, 64, 65, 67, 69, 71, 72};
static int portAudioCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	(void)input;
	(void)timeInfo;
	(void)statusFlags;

	auto out = static_cast<float*>(output);
	auto synth = static_cast<Synth*>(userData);

	for (int i = 0; i < frameCount; ++i) {
		if (count % (SAMPLE_RATE / 4) == 0) {
			if (pressing) {
				synth->keyPressed(notes[key]);
				key += inc;
			} else {
				synth->keyReleased(notes[key]);
			}

			if (key == 0) {
				inc = 1;
			} else  if (key == 7) {
				inc = -1;
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

static void midiCallback(double deltaTime, std::vector<unsigned char> *message, void *userData)
{
	std::vector<unsigned char>& messageRef = *message;
	unsigned char status = messageRef[0] >> 4;
	switch (status) {
		case 8:
			std::cout << "Note off! Key: " << messageRef[1] << ", vel: " << messageRef[2] << std::endl;
			break;

		case 9:
			std::cout << "Note on! Key: " << messageRef[1] << ", vel: " << messageRef[2] << std::endl;
			break;

		default:
			std::cout << "Unhandled data with status: " << status << std::endl;
	}
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

void setupRtMidi(RtMidiIn *midiIn, Synth *synth)
{
	try {
		midiIn = new RtMidiIn();
	} catch (RtMidiError &error) {
		error.printMessage();
	}

	unsigned int nPorts = midiIn->getPortCount();
	std::cout << "RtMidi found " << nPorts << " MIDI sources" << std::endl;

	std::string portName;
	for (unsigned int i = 0; i < nPorts; ++i) {
		try {
			portName = midiIn->getPortName(i);
		} catch (RtMidiError &error) {
			error.printMessage();
		}

		std::cout << "Input port " << i + 1 << ": " << portName << std::endl;
	}

	midiIn->openPort(0);
	midiIn->setCallback(&midiCallback, synth);
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

	RtMidiIn *midiIn = nullptr;
	setupRtMidi(midiIn, synth);

	while (true);

	// delete midiIn;
	// delete oscillator;

	return 0;
}