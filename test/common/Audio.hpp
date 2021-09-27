/*
** EPITECH PROJECT, 2021
** Audio
** File description:
** Audio header
*/

#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include "portaudio.h"
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (5)
#define NUM_CHANNELS    (2)
#define DITHER_FLAG     (0)
#define WRITE_TO_FILE   (0)

#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  0.0f
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  0
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  0
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#endif

typedef struct
{
    int          frameIndex;
    int          maxFrameIndex;
    SAMPLE      *recordedSamples;
} paTestData;

static int recordCallback(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *);
static int playCallback(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *);

class Audio
{
public:
    Audio();
    int play();
    int record();
    ~Audio();
private:
    paTestData data;
};

#endif
