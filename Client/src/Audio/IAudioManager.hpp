/*
** EPITECH PROJECT, 2021
** IAudioManager.hpp
** File description:
** Abstraction class of a Audio library
*/

#pragma once

#include <vector>
#include <cstdint>

namespace Babel
{
	//! @brief Audio Module Interface
	class IAudioManager
	{
	public:
		//! @brief get the number of channels of Input Device for future streams
		virtual int32_t getInputChannelsNumber() const = 0;

		//! @brief get the number of channels of Output Device for future streams
		virtual int32_t getOutputChannelsNumber() const = 0;

		//! @brief set the number of channels of Output for future streams
		virtual void setOutputChannelsNumber(int32_t) = 0;

		//! @brief set the number of channels of Output for future streams
		virtual void setInputChannelsNumber(int32_t) = 0;

		//! @brief get the number of channels for future streams
		virtual int32_t getChannelsNumber() const = 0;

		//! @brief set the samplerate for future streams
		virtual void setSampleRate(int32_t) = 0;

		//! @brief get the current samplerate
		virtual int32_t getSampleRate() const = 0;

		//! @brief get the record time for readStreams
		virtual int32_t getRecordTime() const = 0;

		//! @brief get the record time for readStreams
		virtual void setRecordTime(int32_t) = 0;

		//! @brief get the frame per buffer value for readStreams
		virtual int32_t getFramesPerBuffer() const = 0;

		//! @brief get the frame per buffer for readStreams
		virtual void setFramesPerBuffer(int32_t) = 0;

		//! @brief Open stream to input and output sounds
		virtual void openStream() = 0;

		//! @brief Start a stream
		virtual void startStream() = 0;

		//! @brief Stop a stream
		virtual void stopStream() = 0;

		//! @brief Close current stream
		virtual void closeStream() = 0;

		//! @brief Read an array of int16_t from current stream
		virtual std::vector<int16_t> readStream() = 0;

		//! @brief Write an array of int16_t to current stream
		virtual void writeStream(std::vector<int16_t> &) = 0;

		//! @brief virtual destructor
		virtual ~IAudioManager() = default;
	};
}
