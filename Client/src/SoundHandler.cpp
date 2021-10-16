//
// SoundHandler.cpp
//

#include "Network/NetworkException.hpp"
#include "SoundHandler.hpp"

Babel::SoundHandler::SoundHandler(std::int16_t port)
{
    this->_audio = std::make_shared<Babel::PortAudio>();
    this->_codec = std::make_shared<Babel::Opus>();
    this->_socket = std::make_shared<Babel::UDPSocket>(port, this->_audio, this->_codec);
}

Babel::SoundHandler::~SoundHandler()
{
}

void Babel::SoundHandler::startCall(void)
{
    this->_audio->startStream();
}

void Babel::SoundHandler::stopCall(void)
{
    this->_audio->stopStream();
}

