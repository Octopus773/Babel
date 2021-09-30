#include "Opus.hpp"
#include "OpusException.hpp"

Babel::Opus::Opus() : _encoderIsInitialized(false), _decoderIsInitialized(false), _bitrate(48000),
                      _channel(2), _application(OPUS_APPLICATION_VOIP), _frameSize(960), _dataSize(4000) {
    int err = 0;

    if (_encoder == nullptr) {
        _encoder = opus_encoder_create(_bitrate, _channel, _application, &err);
        if (err < 0) {
            throw OpusException("Could not create the Opus encoder: " + std::string(opus_strerror(err)));
        } else {
            _encoderIsInitialized = true;
        }
    }
    if (_decoder == nullptr) {
        _decoder = opus_decoder_create(_bitrate, _channel, &err);
        if (err < 0) {
            OpusException("Could not create the Opus decoder: " + std::string(opus_strerror(err)));
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

int Babel::Opus::getFrameSize() const {
    return _frameSize;
}

void Babel::Opus::setFrameSize(int frameSize) {
    _frameSize = frameSize;
}

bool Babel::Opus::isEncoderInitialized() const {
    return _encoderIsInitialized;
}

bool Babel::Opus::isDecoderInitialized() const {
    return _decoderIsInitialized;
}



int Babel::Opus::Encode(const std::int16_t *pcm, unsigned char *data) {
    if (_encoderIsInitialized) {
        std::int32_t bytesRead = opus_encode(_encoder, pcm, _frameSize, data, _dataSize);
        if (bytesRead < 0) {
            throw OpusException("Error when encoding with Opus: " + std::string(opus_strerror(bytesRead)));
        }
        return bytesRead;
    } else {
        throw OpusException("Error when encoding: encoder is not initialized");
    }
}

int Babel::Opus::Encode(const float *pcm, unsigned char *data) {
    if (_encoderIsInitialized) {
        std::int32_t bytesRead = opus_encode_float(_encoder, pcm, _frameSize, data, _dataSize);
        if (bytesRead < 0) {
            throw OpusException("Error when encoding with Opus: " + std::string(opus_strerror(bytesRead)));
        }
        return bytesRead;
    } else {
        throw OpusException("Error when encoding: encoder is not initialized");
    }
}

int Babel::Opus::Decode(const unsigned char *data, std::int16_t *pcm) {
    int decodedFrames;

    if (_decoderIsInitialized) {
        if (data == nullptr) {
            decodedFrames = opus_decode(_decoder, nullptr, 0, pcm, _frameSize, 0);
        } else {
            decodedFrames = opus_decode(_decoder, data, _dataSize, pcm, _frameSize, 0);
        }
        if (decodedFrames < 0) {
            throw OpusException("Error when encoding with Opus: " + std::string(opus_strerror(decodedFrames)));
        } else {
            return decodedFrames;
        }
    } else {
        throw OpusException("Error when decoding: decoder is not initialized");
    }
}

int Babel::Opus::Decode(const unsigned char *data, float *pcm) {
    int decodedFrames;

    if (_decoderIsInitialized) {
        if (data == nullptr) {
            decodedFrames = opus_decode_float(_decoder, nullptr, 0, pcm, _frameSize, 0);
        } else {
            decodedFrames = opus_decode_float(_decoder, data, _dataSize, pcm, _frameSize, 0);
        }
        if (decodedFrames < 0) {
            throw OpusException("Error when encoding with Opus: " + std::string(opus_strerror(decodedFrames)));
        } else {
            return decodedFrames;
        }
    } else {
        throw OpusException("Error when decoding: decoder is not initialized");
    }
}