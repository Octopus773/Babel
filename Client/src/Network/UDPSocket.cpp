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
#include "SoundHandler.hpp"
#include "AudioPacket.hpp"

void onError()
{
    std::cout << "Error when receiving packet" << std::endl;
}

Babel::UDPSocket::UDPSocket(std::int16_t port, std::shared_ptr<Babel::IAudioManager> audio, std::shared_ptr<Babel::ICodec> opus)
    : _audio(audio), _codec(opus), _port(port)
{
    std::lock_guard<std::mutex> lockGuard(_mutex);
    this->_socket = std::make_unique<QUdpSocket>(this);
    if (!this->_socket->bind(QHostAddress::AnyIPv4, port))
        throw NetworkException("UDPSocket: Cannot bind to port");
    connect(_socket.get(), &QUdpSocket::readyRead, this, &UDPSocket::readPending);
    _clock = std::chrono::system_clock::now();
}

std::int64_t Babel::UDPSocket::write(std::array<unsigned char, 4000> &encoded, std::int32_t size, const std::string &address, int port)
{
    AudioPacket packet(encoded, size);
    char toSend[sizeof(AudioPacket)];
    std::memcpy(toSend, &packet, sizeof(packet));

    //std::cout << "[ENVOI] Timestamp = " << packet.timestamp << " & size = " << packet.size << std::endl;
    //std::cout << "A envoyé taille = " << sizeof(packet) << std::endl;
    //for (int i = 0; i < sizeof(toSend); i++) {
    //    std::cout << toSend[i];
    //}
    //std::cout << std::endl;

    std::int64_t result = _socket->writeDatagram(toSend, sizeof(toSend), QHostAddress(address.c_str()), port);
    return result;
}

void Babel::UDPSocket::readPending()
{
    if (this->_socket->hasPendingDatagrams()) {
        //std::cout << "Received packets" << std::endl;
        QNetworkDatagram datagram = this->_socket->receiveDatagram();

        // déballe le paquet
        auto *packetRecu = reinterpret_cast<Babel::AudioPacket *> (datagram.data().data());
        char *debugme = datagram.data().data();
        std::uint64_t timestamp = packetRecu->timestamp;
        std::int32_t sizeRecu = packetRecu->size;
        std::vector<unsigned char> encodedReceived(sizeRecu);
        std::memcpy(encodedReceived.data(), packetRecu->data, sizeRecu);
        //std::cout << "[RECU] Timestamp = " << packetRecu->timestamp << " & size = " << packetRecu->size << std::endl;
        //std::cout << "A recu taille = " << sizeof(*packetRecu) << std::endl;
        //for (int i = 0; i < sizeRecu; i++) {
        //    std::cout << packetRecu->data[i];
        //}
        //std::cout << std::endl;

        // on décode
        std::vector<std::int16_t> decodedData(_audio->getFramesPerBuffer() * _audio->getInputChannelsNumber(), 0);
        _codec->decode(encodedReceived.data(), decodedData.data(), sizeRecu);
        try {
            _audio->writeStream(decodedData);
        } catch (const std::exception &e) {
        }
    }
}

Babel::UDPSocket::~UDPSocket() {
    std::lock_guard<std::mutex> lockGuard(_mutex);
    this->_socket->close();
}

void Babel::UDPSocket::close() {
    std::lock_guard<std::mutex> lockGuard(_mutex);
    this->_socket->close();
}