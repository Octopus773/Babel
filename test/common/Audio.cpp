/*
** EPITECH PROJECT, 2021
** Audio
** File description:
** Audio cpp file
*/

#include "Audio.hpp"
#include <exception>
#include <cstring>
#include <iostream>
#include "portaudio.h"

Audio::Audio()
{
    int ret = Pa_Initialize();

    if (ret != paNoError) {
        std::cerr << "Error in intializing PortAudio" << std::endl;
        throw std::exception();
    }
}

Audio::~Audio()
{
    int ret = Pa_Terminate();

    if (ret != paNoError)
        std::cerr << "Error in terminating PortAudio" << std::endl;
}

int Audio::record()
{
    int err = paNoError;
    PaStreamParameters inputParameters;
    //PaStreamParameters outputParameters;
    PaStream *stream;
    int totalFrames;
    int numSamples;
    int numBytes;
    std::vector<int16_t> sample;

    std::cout << "Now recording" << std::endl;
    pcm.clear();
    sample = std::vector<int16_t>(FRAMES_PER_BUFFER * 2);
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        std::cerr << "No imput devices found" << std::endl;
        return (-1);
    }
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream(&stream, &inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err != paNoError) {
        std::cerr << "Could not open stream" << std::endl;
        return (-1);
    }
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Could not start stream" << std::endl;
        return (-1);
    }
    for (long i = 0; i < (NUM_SECONDS * SAMPLE_RATE) / FRAMES_PER_BUFFER; i++) {
        err = Pa_ReadStream(stream, sample.data(), FRAMES_PER_BUFFER);
        pcm.insert(pcm.end(), sample.begin(), sample.end());
    }
    err = Pa_StopStream(stream);
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "Could not close stream" << std::endl;
        return (-1);
    }
    return (err);
}

int Audio::play()
{
    int err = paNoError;
    PaStream *stream;
    PaStreamParameters outputParameters;
    long size = 0;
    long index = 0;

    std::cout << "Now recording" << std::endl;
    if (pcm.empty()) {
        std::cerr << "No input to broadcast" << std::endl;
        return (-1);
    }
    data.frameIndex = 0;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "No default output device found" << std::endl;
        return (-1);
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err != paNoError) {
        std::cerr << "Could not open output stream" << std::endl;
        return (-1);
    }
    if (stream) {
        err = Pa_StartStream(stream);
        if (err != paNoError) {
            std::cerr << "Could not start stream"<< std::endl;
            return (-1);
        }
        Pa_WriteStream(stream, pcm.data(), pcm.size() / 2);
    }
    if (err != paNoError) {
        std::cerr << "Could not close output stream" << std::endl;
        return (-1);
    }
    err = Pa_StopStream(stream);
    Pa_CloseStream(stream);
    return (err);
}
