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
		for (int j = 0; j < CHANNELS; ++j) {
			*out++ = samples[i];
		}
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
			unsigned char controlNumber = messageRef[1];
			if (controlNumber == 7) {
				synth->setMasterVolume(convertRanges(static_cast<double>(messageRef[2]), 0.0, 127.0, 0.0, 1.0));
			} else if (controlNumber == 1) {
				double value = convertRanges(static_cast<double>(messageRef[2]), 0.0, 127.0, 0.0, 1.0);
				synth->pitchBend(value);
			}

			break;
		}

		case 14: {
			// This kind of works but it's more trouble than it's worth right now
			unsigned char lsb = messageRef[1];
			unsigned char msb = messageRef[2];
			unsigned int value = 0;
			value |= msb;
			value <<= 7;
			value |= lsb;
			
			int half = value / 2;
			double freqMultiplier = 1.0;
			if (half < INT16_MAX / 2) {
				freqMultiplier = convertRanges(static_cast<double>(value), 0.0, 16383.0/2.0, 0.5, 1.0);
			} else if (half > INT16_MAX / 2) {
				freqMultiplier = convertRanges(static_cast<double>(value), 0.0, 16383.0/2.0, 1.0, 2.0);
			}

			synth->pitchBend(freqMultiplier);

			break;
		}

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
        exit(-1);
    }
    
    error = Pa_OpenDefaultStream(&stream, 0, CHANNELS, paFloat32, SAMPLE_RATE, BUFFER_SIZE, portAudioCallback, userData);
    if (error != paNoError) {
        std::cout << "PortAudio stream error: " << Pa_GetErrorText(error) << std::endl;
        exit(-1);
    }
    
    error = Pa_StartStream(stream);
    if (error != paNoError) {
        std::cout << "PortAudio stream start error: " << Pa_GetErrorText(error) << std::endl;
        exit(-1);
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
	if (nPorts == 0) {
		std::cout << "No MIDI devices found" << std::endl;
		exit(-1);
	}

	// std::cout << "RtMidi found " << nPorts << " MIDI sources" << std::endl;

	std::vector<std::string> midiNames;
	std::string portName;
	for (unsigned int i = 0; i < nPorts; ++i) {
		try {
			portName = midiIn->getPortName(i);
		} catch (RtMidiError &error) {
			error.printMessage();
		}

		midiNames.push_back(portName);
		// std::cout << "Input port " << i + 1 << ": " << portName << std::endl;
	}

	std::cout << "Using MIDI device " << 0 << ": " << midiNames[0] << std::endl;

	midiIn->openPort(0);
	midiIn->setCallback(&midiCallback, synth);
}

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	auto synth = new Synth();

	PaStream *stream = nullptr;
	setupPortAudio(stream, synth);

	const std::string title = "\n" + std::string(
	"    _____       ______     _____             __  __     \n") + std::string(
  	"   / ___/____  / __/ /_   / ___/__  ______  / /_/ /_    \n") + std::string(
  	"   \\__ \\/ __ \\/ /_/ __/   \\__ \\/ / / / __ \\/ __/ __ \\   \n") + std::string(
    "  ___/ / /_/ / __/ /_    ___/ / /_/ / / / / /_/ / / /   \n") + std::string(
    " /____/\\____/_/  \\__/   /____/\\__, /_/ /_/\\__/_/ /_/ \n") + std::string(
    "                             /____/                   \n");

    std::cout << title << std::endl;

	RtMidiIn *midiIn = nullptr;
	setupRtMidi(midiIn, synth);

	while (true);

	// delete midiIn;
	// delete oscillator;

	return 0;
}