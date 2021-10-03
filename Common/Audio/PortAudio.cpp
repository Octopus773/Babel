/*
** EPITECH PROJECT, 2021
** PortAudio.cpp
** File description:
** PortAudio class file
*/

#include "PortAudio.hpp"
#include <iostream>
#include <exception>

Babel::PortAudio::PortAudio()
{
    int err = Pa_Initialize();
    _streamStopped = true;
    _stream = NULL;
    _frames_per_buffer = 960;
    _samplerate = 48000;
    _number_channels = 2;
    _recordtime = 10;

    if (err != paNoError) {
        std::cerr << "Error in intializing PortAudio" << std::endl;
	throw std::exception();
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
        std::cerr << "No imput devices found" << std::endl;
        throw std::exception();
    }
    inputParameters.channelCount = _number_channels;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "No default output device found" << std::endl;
        throw std::exception();
    }
    outputParameters.channelCount = _number_channels;
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream(&_stream, &inputParameters, &outputParameters, _samplerate, _frames_per_buffer, paClipOff, NULL, NULL);
    if (err != paNoError || !_stream) {
        _stream = NULL;
        std::cerr << "Could not open output stream" << std::endl;
        throw std::exception();
    }
}

void Babel::PortAudio::startStream()
{
    int32_t err = paNoError;

    if (!_stream) {
        std::cerr << "Audio stream not set" << std::endl;
        throw std::exception();
    }
    if (!_streamStopped) {
        std::cerr << "Audio stream already started" << std::endl;
        return;
    }
    err = Pa_StartStream(_stream);
    if (err != paNoError) {
        std::cerr << "Error: could not start audio stream" << std::endl;
        throw std::exception();
    }
    _streamStopped = false;
}

void Babel::PortAudio::stopStream()
{
    if (!_stream) {
        std::cerr << "Error: stream is not set" << std::endl;
        return;
    }
    if (_streamStopped) {
        std::cerr << "stream already stopped" << std::endl;
        return;
    }
    if (Pa_StopStream(_stream) != paNoError)
        std::cerr << "Error: could not stop stream" << std::endl;
    _streamStopped = true;
}

void Babel::PortAudio::closeStream()
{
    if (!_stream) {
        std::cerr << "Error: stream is not set" << std::endl;
        return;
    }
    if (!_streamStopped)
        stopStream();
    Pa_CloseStream(_stream);
    _stream = NULL;
}

std::vector<int16_t> Babel::PortAudio::readStream()
{
    std::vector<int16_t> data(_frames_per_buffer * _number_channels);
    int32_t err = paNoError;

    if (!_stream) {
        std::cerr << "Error: stream is not set" << std::endl;
        throw std::exception();
    }
    if (_streamStopped) {
        std::cerr << "Error: stream not started" << std::endl;
        throw std::exception();
    }
    std::fill(data.begin(), data.end(), 0);
    err = Pa_ReadStream(_stream, data.data(), _frames_per_buffer);
    if (err != paNoError) {
        std::cerr << "Error: could not read from stream" << std::endl;
        throw std::exception();
    }
    return (data);
}

void Babel::PortAudio::writeStream(std::vector<int16_t> &data)
{
    int32_t err = paNoError;

    if (!_stream) {
        std::cerr << "Error: stream is not set" << std::endl;
        throw std::exception();
    }
    if (_streamStopped) {
        std::cerr << "Error: stream not started" << std::endl;
        throw std::exception();
    }
    err = Pa_WriteStream(_stream, data.data(), data.size() / _number_channels);
    if (err != paNoError) {
        std::cerr << "Error: could not write to stream" << std::endl;
        throw std::exception();
    }
}

Babel::PortAudio::~PortAudio()
{
    if (_stream) {
        if (!_streamStopped)
            stopStream();
        closeStream();
    }
    Pa_Terminate();
}
