//
// Created by mgl on 10/11/21.
//

#include <QNetworkDatagram>
#include "Audio/Opus/ICodec.hpp"
#include "Audio/IAudioManager.hpp"
#include <QDataStream>
#include <cstring>
#include <memory>
#include "UDPSocket.hpp"
#include "NetworkException.hpp"
#include "SoundHandler.hpp"
#include "AudioPacket.hpp"

Babel::UDPSocket::UDPSocket(std::int16_t port, std::shared_ptr<Babel::IAudioManager> &audio,
                            std::shared_ptr<Babel::ICodec> &opus)
        : _audio(audio), _codec(opus), _port(port) {
    this->_socket = std::make_unique<QUdpSocket>(this);
    if (!this->_socket->bind(QHostAddress::AnyIPv4, port))
        throw NetworkException("UDPSocket: Cannot bind to port");
    connect(_socket.get(), &QUdpSocket::readyRead, this, &UDPSocket::readPending);
    _clock = std::chrono::system_clock::now();
}

QDataStream &operator <<(QDataStream& out, Babel::AudioPacket& data);

std::int64_t
Babel::UDPSocket::write(std::array<unsigned char, 4000> &encoded, std::int32_t size, const std::string &address,
                        int port) {
    AudioPacket packet;

    packet.data = QByteArray(reinterpret_cast<char *>(encoded.data()), size);
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    packet.timestamp = (qint32) (ms.count() % 100000);
    packet.size = (qint32) size;

    QByteArray buf;
    QDataStream str(&buf, QIODevice::WriteOnly);

    str << packet.timestamp << packet.size << packet.data;

    std::int64_t result = _socket->writeDatagram(buf, QHostAddress(address.c_str()), port);
    return result;
}

void Babel::UDPSocket::readPending() {
    if (this->_socket->hasPendingDatagrams()) {
        QHostAddress sender;
        quint16 senderPort;
        AudioPacket packet;

        QNetworkDatagram datagram = this->_socket->receiveDatagram();

        QByteArray buf(this->_socket->pendingDatagramSize(), Qt::Uninitialized);
        QDataStream str(&buf, QIODevice::ReadOnly);

        this->_socket->readDatagram(buf.data(), buf.size(), &sender, &senderPort);

        packet.timestamp = get<qint32>(str);
        packet.size = get<qint32>(str);
        packet.data = get<QByteArray>(str);

        std::vector<std::int16_t> decodedData(_audio->getFramesPerBuffer() * _audio->getInputChannelsNumber(), 0);
        _codec->decode(reinterpret_cast<const unsigned char *>(packet.data.data()), decodedData.data(), packet.size);
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

template<typename T>
T Babel::UDPSocket::get(QDataStream &str) {
        T value;
        str >> value;
        return value;
}
