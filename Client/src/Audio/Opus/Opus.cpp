//
// Created by mgl on 9/29/21.
//

#include "Opus.hpp"
#include "OpusException.hpp"

Babel::Opus::Opus() : _encoderIsInitialized(false), _decoderIsInitialized(false), _bitrate(48000),
                    _channel(2), _application(OPUS_APPLICATION_VOIP)
{
    int err = 0;

    if (_encoder == nullptr) {
        _encoder = opus_encoder_create(_bitrate, _channel, _application, &err);
        if (err < 0) {
            throw OpusException("Could not create the Opus encoder.");
        } else {
            _encoderIsInitialized = true;
        }
    }
    if (_decoder == nullptr) {
        _decoder = opus_decoder_create(_bitrate, _channel, &err);
        if (err < 0) {
            OpusException("Could not create the Opus decoder.");
        } else {
            _decoderIsInitialized = true;
        }
    }
}

Babel::Opus::~Opus() {
    if (_encoderIsInitialized) {
        opus_encoder_destroy(_encoder);
    }
    if (_decoderIsInitialized) {
        opus_decoder_destroy(_decoder);
    }
}

opus_int32 Babel::Opus::getBitrate() const {
    return _bitrate;
}

void Babel::Opus::setBitrate(opus_int32 bitrate) {
    _bitrate = bitrate;
}

int Babel::Opus::getChannel() const {
    return _channel;
}

void Babel::Opus::setChannel(int channel) {
    _channel = channel;
}

int Babel::Opus::getApplication() const {
    return _application;
}

void Babel::Opus::setApplication(int application) {
    _application = application;
}

int Babel::Opus::Encode(const std::int16_t *pcm, int frameSize, unsigned char *data, std::int32_t dataSize) {
    if (_encoderIsInitialized) {
        std::int32_t bytesRead = opus_encode(_encoder, pcm, frameSize, data, dataSize);
        if (bytesRead < 0) {

        }
    }
}
