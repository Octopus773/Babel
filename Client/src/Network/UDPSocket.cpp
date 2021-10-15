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

void onError()
{
    std::cout << "Error when receiving packet" << std::endl;
}

Babel::UDPSocket::UDPSocket(std::int16_t port, std::shared_ptr<Babel::IAudioManager> audio, std::shared_ptr<Babel::ICodec> opus)
    : _audio(audio), _codec(opus), _port(port)
{
    this->_socket = std::make_unique<QUdpSocket>(this);
    if (!this->_socket->bind(QHostAddress::AnyIPv4, port))
        throw NetworkException("UDPSocket: Cannot bind to port");
    connect(_socket.get(), &QUdpSocket::readyRead, this, &UDPSocket::readPending);
    _clock = std::chrono::system_clock::now();
}

std::int64_t Babel::UDPSocket::write(std::array<unsigned char, 4000> &data, const std::string &address, int port)
{
    AudioPacket packet(data);
    char toSend[sizeof(AudioPacket)];
    std::memcpy(toSend, &packet, sizeof(packet));

    std::int64_t result = _socket->writeDatagram(toSend, sizeof(toSend), QHostAddress(address.c_str()), port);
    //std::cout << "Sent " << result << " sized packet to " << address << ":" << port << std::endl;
    return result;
}

void Babel::UDPSocket::readPending()
{
    while (this->_socket->hasPendingDatagrams()) {
        //std::cout << "Received packets" << std::endl;
        QNetworkDatagram datagram = this->_socket->receiveDatagram();

        AudioPacket *packet = (AudioPacket *) (datagram.data().data());

        std::uint64_t timestamp = packet->timestamp;
        const std::int32_t size = packet->size;

        std::vector<unsigned char> encoded(size);
        std::memcpy(encoded.data(), packet->data, size);

        _inputBuffer[timestamp] = encoded;

        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(_clock - std::chrono::system_clock::now());

        std::cout << milliseconds.count() << std::endl;

        if (abs(milliseconds.count()) >= 20) {
            std::cout << "Flushing buffer" << std::endl;
            for (auto &payload : _inputBuffer) {
                std::vector<std::int16_t> decodedData(_audio->getFramesPerBuffer() * _audio->getInputChannelsNumber(), 0);
                    _codec->decode(payload.second.data(), decodedData.data(), decodedData.size());
                    try
                    {
                        _audio->writeStream(decodedData);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
            }
            _inputBuffer.clear();
            _clock = std::chrono::system_clock::now();
        }
    }
}

Babel::UDPSocket::~UDPSocket() {
    this->_socket->close();
}

void Babel::UDPSocket::close() {
    this->_socket->close();
}