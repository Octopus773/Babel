//
// Created by mgl on 10/11/21.
//

#include <QNetworkDatagram>
#include "Audio/Opus/ICodec.hpp"
#include "Audio/IAudioManager.hpp"
#include <cstring>
#include <memory>
#include "UDPSocket.hpp"
#include "NetworkException.hpp"
#include "SoundHandler.hpp"
#include "AudioPacket.hpp"

Babel::UDPSocket::UDPSocket(std::int16_t port, std::shared_ptr<Babel::IAudioManager> &audio,
                            std::shared_ptr<Babel::ICodec> &opus)
	: _audio(audio),
	  _codec(opus)
{
	this->_socket = std::make_unique<QUdpSocket>(this);
	if (!this->_socket->bind(QHostAddress::AnyIPv4, port))
		throw NetworkException("UDPSocket: Cannot bind to port");
	QObject::connect(_socket.get(), &QUdpSocket::readyRead, this, &UDPSocket::readPending);
}

std::int64_t
Babel::UDPSocket::write(std::array<unsigned char, 4000> &encoded, std::int32_t size, const std::string &address,
                        int port) {
    AudioPacket packet(encoded, size);
    char toSend[sizeof(AudioPacket)];
    std::memcpy(toSend, &packet, sizeof(packet));

    std::int64_t result = _socket->writeDatagram(toSend, sizeof(toSend), QHostAddress(address.c_str()), port);
    return result;
}

void Babel::UDPSocket::readPending() {
    if (this->_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = this->_socket->receiveDatagram();

        auto *receivedPacket = reinterpret_cast<Babel::AudioPacket *> (datagram.data().data());
        //std::uint64_t timestamp = receivedPacket->timestamp;
        std::int32_t sizeRecu = receivedPacket->size;
        std::vector<unsigned char> encodedReceived(sizeRecu);
        std::memcpy(encodedReceived.data(), receivedPacket->data, sizeRecu);

        std::vector<std::int16_t> decodedData(_audio->getFramesPerBuffer(), 0);
        _codec->decode(encodedReceived.data(), decodedData.data(), sizeRecu);
        try {
            this->_audio->writeStream(decodedData);
        } catch (const std::exception &e) {
        }
    }
}

Babel::UDPSocket::~UDPSocket() {
    this->close();
}

void Babel::UDPSocket::close() {
    std::lock_guard<std::mutex> lockGuard(_mutex);
    this->_socket->close();
}

uint16_t Babel::UDPSocket::getLocalPort() const
{
	return this->_socket->localPort();
}