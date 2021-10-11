//
// Created by mgl on 10/11/21.
//

#ifndef BABEL_ICODEC_HPP
#define BABEL_ICODEC_HPP

namespace Babel {
    class ICodec {
    public:
        virtual ~ICodec() = default;

        //! @brief Encodes exactly one frame of audio data
        //! @param pcm the frame to encode
        //! @param data encoded output
        virtual int encode(const std::int16_t *pcm, unsigned char *data) = 0;

        //! @brief same as Encode() with floats
        virtual int encode(const float *pcm, unsigned char *data) = 0;

        //! @brief Decodes exactly one frame of audio data
        //! @param data the frame to decode, can be nullptr in case of packet loss
        //! @param pcm decoded data
        //! @return number of samples decoded
        virtual int decode(const unsigned char *data, std::int16_t *pcm, std::int32_t dataSize) = 0;

        //! @brief Same as Decode() with floats
        virtual int decode(const unsigned char *data, float *pcm) = 0;

        //! @brief get the bitrate
        [[nodiscard]] virtual std::int32_t getBitrate() const = 0;

        //! @brief Sets the bitrate
        virtual void setBitrate(std::int32_t bitrate) = 0;

        //! @brief get the channels
        [[nodiscard]] virtual int getChannel() const = 0;

        //! @brief sets the channel
        virtual void setChannel(int channel) = 0;

        //! @brief get the application type
        [[nodiscard]] virtual int getApplication() const = 0;

        //! @brief sets the Opus application mode
        //! Possible values are OPUS_APPLICATION_AUDIO (default)
        //! OPUS_APPLICATION_VOIP
        //! OPUS_APPLICATION_RESTRICTED_LOWDELAY
        virtual void setApplication(int application) = 0;

        //! @brief get the frame size
        [[nodiscard]] virtual int getFrameSize() const = 0;

        //! @brief sets the number of samples per channel
        //! Possible values are 120/480/960 (default)/1920/2880
        virtual void setFrameSize(int frameSize) = 0;
    };
}

#endif //BABEL_ICODEC_HPP
