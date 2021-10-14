//
// Created by mgl on 10/11/21.
//

#include <QNetworkDatagram>
#include <utility>
#include <cstring>
#include "UDPSocket.hpp"
#include "NetworkException.hpp"
#include "AudioPacket.hpp"

Babel::UDPSocket::UDPSocket(std::string address, std::int16_t port, std::shared_ptr<Babel::Opus> opus, std::mutex &mtx)
    : _opus(std::move(opus)), _mtx(mtx), _address(std::move(address)), _port(port)
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
    auto ms = duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    //packet.timestamp = ms;

    //return _socket->writeDatagram(data.data(), data.size(), QHostAddress(address.c_str()), port);
}

void Babel::UDPSocket::readPending()
{
    while (this->_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = this->_socket->receiveDatagram();

        // TODO: ordre, doublon, decodage puis lecture buffer
        _mtx.lock();
        // std::vector<std::int16_t> decodedData(a->getFramesPerBuffer() * a->getInputChannelsNumber(), 0);
        // opus->decode(decoded.data(), decodedData.data(), encodedSize);
        // a->writeStream(decodedData);
        _mtx.unlock();
    }
}

Babel::UDPSocket::~UDPSocket() {
    this->_socket->close();
}

void Babel::UDPSocket::close() {
    this->_socket->close();
}