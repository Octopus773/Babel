/*
** EPITECH PROJECT, 2021
** PortAudio.cpp
** File description:
** PortAudio class file
*/

#include "PortAudio.hpp"
#include "Exceptions/BabelException.hpp"
#include <iostream>
#include <exception>

Babel::PortAudio::PortAudio()
{
	int err = Pa_Initialize();
	this->_streamStopped = true;
	this->_stream = nullptr;
	this->_frames_per_buffer = 960;
	this->_samplerate = 48000;
	this->_number_channels = 2;
	this->_recordtime = 10;

	if (err != paNoError) {
		throw Exception::BabelException("Error in intializing PortAudio");
	}
}

void Babel::PortAudio::setChannelsNumber(int32_t nb)
{
	this->_number_channels = nb;
}

int32_t Babel::PortAudio::getChannelsNumber() const
{
	return (this->_number_channels);
}

void Babel::PortAudio::setFramesPerBuffer(int32_t nb)
{
	this->_frames_per_buffer = nb;
}

int32_t Babel::PortAudio::getFramesPerBuffer() const
{
	return (this->_frames_per_buffer);
}

void Babel::PortAudio::setSampleRate(int32_t nb)
{
	this->_samplerate = nb;
}

int32_t Babel::PortAudio::getSampleRate() const
{
	return (this->_samplerate);
}

void Babel::PortAudio::setRecordTime(int32_t nb)
{
	this->_recordtime = nb;
}

int32_t Babel::PortAudio::getRecordTime() const
{
	return (this->_recordtime);
}

void Babel::PortAudio::openStream()
{
	int32_t err = paNoError;
	PaStreamParameters inputParameters;
	PaStreamParameters outputParameters;

	inputParameters.device = Pa_GetDefaultInputDevice();
	if (inputParameters.device == paNoDevice) {
		throw Exception::BabelException("No imput devices found");
	}
	inputParameters.channelCount = this->_number_channels;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = nullptr;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice) {
		throw Exception::BabelException("No default output device found");
	}
	outputParameters.channelCount = this->_number_channels;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = nullptr;
	err = Pa_OpenStream(&this->_stream, &inputParameters, &outputParameters, this->_samplerate, this->_frames_per_buffer, paClipOff, nullptr,
	                    nullptr);
	if (err != paNoError || !this->_stream) {
		this->_stream = nullptr;
		throw Exception::BabelException("Could not open output stream");
	}
}

void Babel::PortAudio::startStream()
{
	int32_t err = paNoError;

	if (!this->_stream) {
		throw Exception::BabelException("Audio stream not set");
	}
	if (!this->_streamStopped) {
		std::cerr << "Audio stream already started" << std::endl;
		return;
	}
	err = Pa_StartStream(this->_stream);
	if (err != paNoError) {
		throw Exception::BabelException("Error: could not start audio stream");
	}
	this->_streamStopped = false;
}

void Babel::PortAudio::stopStream()
{
	if (!this->_stream) {
		std::cerr << "Error: stream is not set" << std::endl;
		return;
	}
	if (this->_streamStopped) {
		std::cerr << "stream already stopped" << std::endl;
		return;
	}
	if (Pa_StopStream(this->_stream) != paNoError)
		std::cerr << "Error: could not stop stream" << std::endl;
	this->_streamStopped = true;
}

void Babel::PortAudio::closeStream()
{
	if (!this->_stream) {
		std::cerr << "Error: stream is not set" << std::endl;
		return;
	}
	if (!this->_streamStopped)
		this->stopStream();
	Pa_CloseStream(this->_stream);
	this->_stream = NULL;
}

std::vector<int16_t> Babel::PortAudio::readStream()
{
	std::vector<int16_t> data(this->_frames_per_buffer * this->_number_channels);
	int32_t err = paNoError;

	if (!this->_stream) {
		throw Exception::BabelException("Error: stream is not set");
	}
	if (this->_streamStopped) {
		throw Exception::BabelException("Error: stream not started");
	}
	std::fill(data.begin(), data.end(), 0);
	err = Pa_ReadStream(this->_stream, data.data(), this->_frames_per_buffer);
	if (err != paNoError) {
		throw Exception::BabelException("Error: could not read from stream");
	}
	return (data);
}

void Babel::PortAudio::writeStream(std::vector<int16_t> &data)
{
	int32_t err = paNoError;

	if (!this->_stream) {
		throw Exception::BabelException("Error: stream is not set");
	}
	if (this->_streamStopped) {
		throw Exception::BabelException("Error: stream not started");
	}
	err = Pa_WriteStream(this->_stream, data.data(), data.size() / this->_number_channels);
	if (err != paNoError) {
		throw Exception::BabelException("Error: could not write to stream");
	}
}

Babel::PortAudio::~PortAudio()
{
	if (this->_stream) {
		if (!this->_streamStopped)
			this->stopStream();
		this->closeStream();
	}
	Pa_Terminate();
}
