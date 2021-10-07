#include "Opus.hpp"
#include "OpusException.hpp"
#include <iostream>

Babel::Opus::Opus() : _encoderIsInitialized(false), _decoderIsInitialized(false), _bitrate(48000),
                      _channel(2), _application(OPUS_APPLICATION_VOIP), _frameSize(960), _dataSize(4000) {
    int err = 0;

    this->_encoder = opus_encoder_create(this->_bitrate, this->_channel, this->_application, &err);
    if (err < 0) {
        throw OpusException("Could not create the Opus encoder: " + std::string(opus_strerror(err)));
    } else {
        this->_encoderIsInitialized = true;
    }
    this->_decoder = opus_decoder_create(this->_bitrate, this->_channel, &err);
    if (err < 0) {
        OpusException("Could not create the Opus decoder: " + std::string(opus_strerror(err)));
    } else {
        this->_decoderIsInitialized = true;
    }
}

Babel::Opus::~Opus() {
    if (this->_encoderIsInitialized) {
        opus_encoder_destroy(this->_encoder);
    }
    if (this->_decoderIsInitialized) {
        opus_decoder_destroy(this->_decoder);
    }
}

opus_int32 Babel::Opus::getBitrate() const {
    return this->_bitrate;
}

void Babel::Opus::setBitrate(std::int32_t bitrate) {
    this->_bitrate = bitrate;
}

int Babel::Opus::getChannel() const {
    return this->_channel;
}

void Babel::Opus::setChannel(int channel) {
    this->_channel = channel;
}

int Babel::Opus::getApplication() const {
    return this->_application;
}

void Babel::Opus::setApplication(int application) {
    this->_application = application;
}

int Babel::Opus::getFrameSize() const {
    return this->_frameSize;
}

void Babel::Opus::setFrameSize(int frameSize) {
    this->_frameSize = frameSize;
}

bool Babel::Opus::isEncoderInitialized() const {
    return this->_encoderIsInitialized;
}

bool Babel::Opus::isDecoderInitialized() const {
    return this->_decoderIsInitialized;
}


int Babel::Opus::encode(const std::int16_t *pcm, unsigned char *data) {
    if (this->_encoderIsInitialized) {
        std::int32_t bytesRead = opus_encode(this->_encoder, pcm, this->_frameSize, data, this->_dataSize);
        if (bytesRead < 0) {
            throw OpusException("Error when encoding with Opus: " + std::string(opus_strerror(bytesRead)));
        }
        return bytesRead;
    } else {
        throw OpusException("Error when encoding: encoder is not initialized");
    }
}

int Babel::Opus::encode(const float *pcm, unsigned char *data) {
    if (this->_encoderIsInitialized) {
        std::int32_t bytesRead = opus_encode_float(this->_encoder, pcm, this->_frameSize, data, this->_dataSize);
        if (bytesRead < 0) {
            throw OpusException("Error when encoding with Opus: " + std::string(opus_strerror(bytesRead)));
        }
        return bytesRead;
    } else {
        throw OpusException("Error when encoding: encoder is not initialized");
    }
}

int Babel::Opus::decode(const unsigned char *data, std::int16_t *pcm, std::int32_t dataSize) {
    int decodedFrames;

    if (this->_decoderIsInitialized) {
        if (data == nullptr) {
            decodedFrames = opus_decode(this->_decoder, nullptr, 0, pcm, this->_frameSize, 0);
        } else {
            decodedFrames = opus_decode(this->_decoder, data, dataSize, pcm, this->_frameSize, 0);
        }
        std::cout << "[OPUS] Decoded " << decodedFrames << " frames" << std::endl;
        if (decodedFrames < 0) {
            throw OpusException("Error when encoding with Opus: " + std::string(opus_strerror(decodedFrames)));
        } else {
            return decodedFrames;
        }
    } else {
        throw OpusException("Error when decoding: decoder is not initialized");
    }
}

int Babel::Opus::decode(const unsigned char *data, float *pcm) {
    int decodedFrames;

    if (this->_decoderIsInitialized) {
        if (data == nullptr) {
            decodedFrames = opus_decode_float(this->_decoder, nullptr, 0, pcm, this->_frameSize, 0);
        } else {
            decodedFrames = opus_decode_float(this->_decoder, data, this->_dataSize, pcm, this->_frameSize, 0);
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