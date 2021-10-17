//
// SoundHandler.cpp
//

#include "Network/NetworkException.hpp"
#include "Network/QtTCPConnection.hpp"
#include "Network/RFCCodes.hpp"
#include "Network/UDPSocket.hpp"
#include "Network/AudioPacket.hpp"
#include "Audio/PortAudio.hpp"
#include "Audio/Opus/Opus.hpp"
#include "SoundHandler.hpp"
#include <exception>

Babel::SoundHandler::SoundHandler(std::int16_t port) : _shouldExit(false), _shouldPlay(false) {
    this->_audio = std::make_shared<Babel::PortAudio>();
    this->_codec = std::make_shared<Babel::Opus>();
    this->_socket = std::make_shared<Babel::UDPSocket>(port, this->_audio, this->_codec);

    this->_audio->openStream();
    this->_execthread = std::thread([this]() {
        std::array<unsigned char, 4000> encoded{0};
        std::vector<int16_t> pcm;

        std::cout << "hello" << std::endl;
        while (true) {
            //std::cout << "Checking exit" << std::endl;
            this->_exit_mtx.lock();
            if (this->_shouldExit) {
                this->_exit_mtx.unlock();
                //std::cerr << "Exiting" << std::endl;
                return;
            }
            //std::cout << "not exit" << std::endl;
            this->_exit_mtx.unlock();
            this->_play_mtx.lock();
            if (!this->_shouldPlay) {
                this->_play_mtx.unlock();
                continue;
            }
            //std::cout << "playing sound" << std::endl;
            this->_play_mtx.unlock();
            try {
                std::vector<int16_t> data = this->_audio->readStream();
                std::int32_t size = this->_codec->encode(data.data(), encoded.data());
                this->_userlist_mtx.lock();
                for (auto it = this->_userlist.begin(); it != this->_userlist.end(); it++) {
                    this->_socket->write(encoded, size, it->second.first, it->second.second);
                }
                this->_userlist_mtx.unlock();
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    });
}

Babel::SoundHandler::~SoundHandler() {
    this->stopCall();
    this->_exit_mtx.lock();
    this->_shouldExit = true;
    this->_exit_mtx.unlock();
    if (this->_execthread.joinable()) {
        this->_execthread.join();
    }
    this->_audio->closeStream();
    std::cout << "killed" << std::endl;
}

void Babel::SoundHandler::startCall(void) {
    this->_audio->startStream();
    std::scoped_lock lock(this->_play_mtx);
    this->_shouldPlay = true;
}

void Babel::SoundHandler::stopCall(void) {
    std::scoped_lock lock1(this->_play_mtx);
    this->_shouldPlay = false;
    this->_audio->stopStream();
    std::scoped_lock lock2(this->_userlist_mtx);
    this->_userlist.clear();
}

void Babel::SoundHandler::addClient(const std::string userid, const std::string address, std::uint16_t port) {
    std::scoped_lock lock(this->_userlist_mtx);
    std::cout << "adding " << address << ":" << port << std::endl;
    this->_userlist[userid] = std::make_pair(address, port);
}

void Babel::SoundHandler::removeClient(const std::string userid) {
    std::scoped_lock lock(this->_userlist_mtx);
    this->_userlist.erase(userid);
}