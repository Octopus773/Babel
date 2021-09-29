//
// Created by mgl on 9/29/21.
//

#ifndef BABEL_OPUS_HPP
#define BABEL_OPUS_HPP

#include <cstdint>
#include "opus/opus.h"

namespace Babel {
    class Opus {
    public:
        Opus();
        ~Opus();
        int Encode(const std::int16_t *pcm, int frameSize, unsigned char *data, std::int32_t dataSize);
        Decode();

    public:
        opus_int32 getBitrate() const;
        void setBitrate(opus_int32 bitrate);

        int getChannel() const;
        void setChannel(int channel);

        int getApplication() const;
        void setApplication(int application);

    private:
        OpusEncoder *_encoder;
        bool _encoderIsInitialized;
        bool _decoderIsInitialized;
        OpusDecoder *_decoder;
        opus_int32 _bitrate;
        int _channel;
        int _application;
    };
}


#endif //BABEL_OPUS_HPP
