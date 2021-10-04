/*
** EPITECH PROJECT, 2021
** PortAudio
** File description:
** encapsulation of the portaudio library for sound management
*/

#pragma once

#include "IAudioManager.hpp"
#include "portaudio.h"

#define PA_SAMPLE_TYPE paInt16

namespace Babel
{
	class PortAudio : public IAudioManager
	{
	public:
		PortAudio();

		void setChannelsNumber(int32_t);

		int32_t getChannelsNumber(void) const;

		void setFramesPerBuffer(int32_t);

		int32_t getFramesPerBuffer() const;

		void setSampleRate(int32_t);

		int32_t getSampleRate() const;

		void setRecordTime(int32_t);

		int32_t getRecordTime() const;

		void openStream();

		void startStream();

		void stopStream();

		void closeStream();

		std::vector<int16_t> readStream();

		void writeStream(std::vector<int16_t> &);

		~PortAudio();

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
