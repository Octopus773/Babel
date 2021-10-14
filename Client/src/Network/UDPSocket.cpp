//
// Created by mgl on 10/11/21.
//

#include <QNetworkDatagram>
#include "Audio/Opus/ICodec.hpp"
#include "Audio/IAudioManager.hpp"
#include <cstring>
#include <memory>
#include <iostream>
#include "UDPSocket.hpp"
#include "NetworkException.hpp"
#include "AudioPacket.hpp"

Babel::UDPSocket::UDPSocket(std::string address, std::int16_t port, std::shared_ptr<Babel::IAudioManager> audio, std::shared_ptr<Babel::ICodec> opus, std::mutex &audio_mtx, std::mutex &codec_mtx)
    : _address(address), _port(port), _audio(audio), _codec(opus), _audio_mtx(audio_mtx), _codec_mtx(codec_mtx)
{
    this->_socket = std::make_unique<QUdpSocket>(this);
    if (!this->_socket->bind(QHostAddress(_address.c_str()), _port))
        throw NetworkException("UDPSocket: Cannot bind to port");
    connect(this->_socket.get(), &QUdpSocket::readyRead, this, &UDPSocket::readPending);
}

std::int64_t Babel::UDPSocket::read(const std::shared_ptr<char>& data, std::int64_t maxSize)
{
    return this->_socket->read(data.get(), maxSize);
}

std::int64_t Babel::UDPSocket::write(std::array<unsigned char, 4000> &data, const std::string &address, int port)
{
    AudioPacket packet {};
    std::memcpy(packet.data, data.data(), data.size());
    packet.size = sizeof(data.data());
    std::int64_t milliseconds_since_epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    packet.timestamp = milliseconds_since_epoch;
    char toSend[sizeof(AudioPacket)];
    std::memcpy(toSend, &packet, sizeof(packet));

    return _socket->writeDatagram(toSend, sizeof(toSend), QHostAddress(address.c_str()), port);
}

void Babel::UDPSocket::readPending()
{
    while (this->_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = this->_socket->receiveDatagram();
        std::vector<std::int16_t> decodedData(_audio->getFramesPerBuffer() * _audio->getInputChannelsNumber(), 0);

        AudioPacket *packet = (AudioPacket *) (datagram.data().data());

        //std::uint64_t timestamp = packet->timestamp;
        const std::int32_t size = packet->size;

        std::vector<unsigned char> encoded (size);
        std::memcpy(encoded.data(), packet->data, size);

        try {
            this->_codec_mtx.lock();
            _codec->decode(encoded.data(), decodedData.data(), size);
            this->_codec_mtx.unlock();
            this->_audio_mtx.lock();
            _audio->writeStream(decodedData);
            this->_audio_mtx.unlock();
        } catch (Exception::BabelException &e) {
            //std::cout << "Packet received error when encoding/reading" << std::endl;
        }
    }
}

Babel::UDPSocket::~UDPSocket() {
    this->_socket->close();
}

void Babel::UDPSocket::close() {
    this->_socket->close();
}