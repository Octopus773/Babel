/*
** EPITECH PROJECT, 2021
** Audio
** File description:
** Audio header
*/

#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include "portaudio.h"
#define SAMPLE_RATE  (48000)
#define FRAMES_PER_BUFFER (960)
#define NUM_SECONDS     (10)
#define NUM_CHANNELS    (2)
#define DITHER_FLAG     (0)
#define WRITE_TO_FILE   (0)
#include <vector>
#include <cstdint>

#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  0

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
    std::vector<int16_t> pcm;
};

#endif
