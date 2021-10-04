/*
** EPITECH PROJECT, 2021
** PortAudio
** File description:
** encapsulation of the portaudio library for sound management
*/

#pragma once

#include "IAudioManager.hpp"
#include <portaudio.h>

#define PA_SAMPLE_TYPE paInt16

namespace Babel
{
	class PortAudio : public IAudioManager
	{
	public:
		PortAudio();
		
		//! @brief set the number of channels for future streams
		void setChannelsNumber(int32_t) override;

		//! @brief get the number of channels for future streams
		int32_t getChannelsNumber() const override;

		//! @brief set the samplerate for future streams
		void setSampleRate(int32_t) override;

		//! @brief get the current samplerate
		int32_t getSampleRate() const override;

		//! @brief get the record time for readStreams
		int32_t getRecordTime() const override;

		//! @brief get the record time for readStreams
		void setRecordTime(int32_t) override;

		//! @brief get the frame per buffer value for readStreams
		int32_t getFramesPerBuffer() const override;

		//! @brief get the frame per buffer for readStreams
		void setFramesPerBuffer(int32_t) override;

		//! @brief Open stream to input and output sounds
		void openStream() override;

		//! @brief Start a stream
		void startStream() override;

		//! @brief Stop a stream
		void stopStream() override final;

		//! @brief Close current stream
		void closeStream() override final;

		//! @brief Read an array of int16_t from current stream
		std::vector<int16_t> readStream() override;

		//! @brief Write an array of int16_t to current stream
		void writeStream(std::vector<int16_t> &) override;

		~PortAudio() override;

	private:
		//! @brief stream for sound input and output
		PaStream *_stream;
		//! @brief frames per buffer for readStream
		int32_t _frames_per_buffer;
		//! @brief number of channels
		int32_t _number_channels;
		//@! brief record number in seconds for one frame
		int32_t _recordtime;
		//@! sample rate for input and output streams
		int32_t _samplerate;
		//@! True if the stream doesn't exist or has not been started
		bool _streamStopped;
	};
}
