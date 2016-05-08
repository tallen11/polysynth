#include <iostream>
#include <PortAudio.h>
#include <cmath>
#include <string>
#include <vector>

#include "Constants.hpp"
#include "Oscillator.hpp"
#include "Synth.hpp"
#include "Util.hpp"
#include "Libraries/RtMidi/RtMidi.h"

static int portAudioCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	(void)input;
	(void)timeInfo;
	(void)statusFlags;

	auto out = static_cast<float*>(output);
	auto synth = static_cast<Synth*>(userData);

	auto samples = synth->getNextBuffer(frameCount);

	for (int i = 0; i < frameCount; ++i) {
		*out++ = samples[i];
		*out++ = samples[i];
	}

	return paContinue;
}

static void midiCallback(double deltaTime, std::vector<unsigned char> *message, void *userData)
{
	(void)deltaTime;

	auto synth = static_cast<Synth*>(userData);
	std::vector<unsigned char>& messageRef = *message;
	unsigned char status = messageRef[0] >> 4;
	switch (status) {
		case 8: {
			synth->keyReleased(static_cast<int>(messageRef[1]));
			break;
		}

		case 9: {
			double velocity = convertRanges(static_cast<double>(messageRef[2]), 0.0, 127.0, 0.0, 1.0);
			synth->keyPressed(static_cast<int>(messageRef[1]), velocity);
			break;
		}

		case 11: {
			if (messageRef[1] == 7) {
				synth->setMasterVolume(convertRanges(static_cast<double>(messageRef[2]), 0.0, 127.0, 0.0, 1.0));
			}

			break;
		}

		// case 14:
		// 	// This kind of works but it's more trouble than it's worth right now
		// 	unsigned char lsb = messageRef[1];
		// 	unsigned char msb = messageRef[2];
		// 	unsigned int value = 0;
		// 	value |= msb;
		// 	value <<= 7;
		// 	value |= lsb;
			
		// 	int half = value / 2;
		// 	double freqMultiplier = 1.0;
		// 	if (half < INT16_MAX / 2) {
		// 		freqMultiplier = convertRanges(static_cast<double>(value), 0.0, 16383.0/2.0, 0.5, 1.0);
		// 	} else if (half > INT16_MAX / 2) {
		// 		freqMultiplier = convertRanges(static_cast<double>(value), 0.0, 16383.0/2.0, 1.0, 2.0);
		// 	}

		// 	synth->pitchBend(freqMultiplier);

		// 	break;

		// default:
			// std::cout << "Unhandled data with status: " << (int)status << std::endl;
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
		midiIn = new RtMidiIn(RtMidi::MACOSX_CORE);
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