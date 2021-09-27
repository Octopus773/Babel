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

static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData)
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void)outputBuffer;
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    if(framesLeft < framesPerBuffer) {
        framesToCalc = framesLeft;
        finished = paComplete;
    } else {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }
    if (inputBuffer == NULL) {
	for(i=0; i<framesToCalc; i++) {
            *wptr++ = SAMPLE_SILENCE;
            if(NUM_CHANNELS == 2)
                *wptr++ = SAMPLE_SILENCE;
        }
    } else {
        for( i=0; i<framesToCalc; i++) {
            *wptr++ = *rptr++;
            if(NUM_CHANNELS == 2)
                *wptr++ = *rptr++;
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}

static int playCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    paTestData *data = (paTestData*)userData;
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void)inputBuffer;
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    if(framesLeft < framesPerBuffer) {
        for(i=0; i<framesLeft; i++) {
            *wptr++ = *rptr++;
            if( NUM_CHANNELS == 2 )
                *wptr++ = *rptr++;
        }
        for(; i<framesPerBuffer; i++) {
            *wptr++ = 0;
            if( NUM_CHANNELS == 2 ) *wptr++ = 0;
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    } else {
        for(i=0; i<framesPerBuffer; i++) {
            *wptr++ = *rptr++;
            if(NUM_CHANNELS == 2)
                *wptr++ = *rptr++;
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

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

    data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE;
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);
    data.recordedSamples = (SAMPLE *)malloc(numBytes);
    if (!data.recordedSamples) {
        std::cerr << "Could not allocate data buffer" << std::endl;
        return (-1);
    }
    memset(data.recordedSamples, 0, numSamples);
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        std::cerr << "No imput devices found" << std::endl;
        free(data.recordedSamples);
        return (-1);
    }
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream(&stream, &inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, recordCallback, &data);
    if (err != paNoError) {
        std::cerr << "Could not open stream" << std::endl;
        free(data.recordedSamples);
        return (-1);
    }
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Could not start stream" << std::endl;
        free(data.recordedSamples);
        return (-1);
    }
    while((err = Pa_IsStreamActive(stream)) == 1) {
        Pa_Sleep(1000);
    }
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "Could not close stream" << std::endl;
        free(data.recordedSamples);
        return (-1);
    }
    FILE *fd = fopen("record.raw", "wb");
    if (!fd) {
        std::cerr << "Could not create file record.raw" << std::endl;
        free(data.recordedSamples);
        return (-1);
    } else {
        fwrite( data.recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fd);
        std::cout << "record successfully written" << std::endl;
        fclose(fd);
    }
    return (err);
}

int Audio::play()
{
    int err = paNoError;
    PaStream *stream;
    PaStreamParameters outputParameters;

    if (!data.recordedSamples) {
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
    err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, playCallback, &data);
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
        while (err = Pa_IsStreamActive(stream) == 1)
            Pa_Sleep(100);
    }
    if (err != paNoError) {
        std::cerr << "Could not close output stream" << std::endl;
        return (-1);
    }
    Pa_CloseStream(stream);
    free(data.recordedSamples);
    return (err);
}
