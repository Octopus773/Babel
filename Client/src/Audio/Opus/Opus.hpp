#pragma once

#include <cstdint>
#include "opus/opus.h"

namespace Babel {
    //! @brief Class containing Opus encoder and decoder
    class Opus {
    public:
        Opus(); //! @brief ctor
        ~Opus(); //! @brief dtor

        //! @brief Encodes exactly one frame of audio data
        //! @param pcm the frame to encode
        //! @param data encoded output
        int Encode(const std::int16_t *pcm, unsigned char *data);

        //! @brief same as Encode() with floats
        int Encode(const float *pcm, unsigned char *data);

        //! @brief Decodes exactly one frame of audio data
        //! @param data the frame to decode, can be nullptr in case of packet loss
        //! @param pcm decoded data
        //! @return number of samples decoded
        int Decode(const unsigned char *data, std::int16_t *pcm);

        //! @brief Same as Decode() with floats
        int Decode(const unsigned char *data, float *pcm);


        [[nodiscard]] std::int32_t getBitrate() const;
        void setBitrate(opus_int32 bitrate);

        [[nodiscard]] int getChannel() const;
        void setChannel(int channel);

        [[nodiscard]] int getApplication() const;
        //! @brief sets the Opus application mode
        //! Possible values are OPUS_APPLICATION_AUDIO (default)
        //! OPUS_APPLICATION_VOIP
        //! OPUS_APPLICATION_RESTRICTED_LOWDELAY
        void setApplication(int application);

        [[nodiscard]] int getFrameSize() const;
        //! @brief sets the number of samples per channel
        //! Possible values are 120/480/960 (default)/1920/2880
        void setFrameSize(int frameSize);

        //! @brief false if encoder failed to initialize
        [[nodiscard]] bool isEncoderInitialized() const;
        //! @brief false if decoder failed to initialize
        [[nodiscard]] bool isDecoderInitialized() const;

    private:
        OpusEncoder *_encoder;
        bool _encoderIsInitialized;
        bool _decoderIsInitialized;
        OpusDecoder *_decoder;
        opus_int32 _bitrate;
        int _channel;
        int _application;
        int _frameSize;
        std::int32_t _dataSize;
    };
}