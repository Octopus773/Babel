//
// Created by mgl on 10/11/21.
//

#include <QNetworkDatagram>
#include "UDPSocket.hpp"
#include "NetworkException.hpp"

Babel::UDPSocket::UDPSocket(const QHostAddress &address, std::int16_t port) : _address(address), _port(port)
{
    this->_socket = std::make_unique<QUdpSocket>(this);
    if (!this->_socket->bind(address, port))
        throw NetworkException("UDPSocket: Cannot bind to port");
    connect(this->_socket.get(), &QUdpSocket::readyRead, this, &UDPSocket::readPending);
}

std::int64_t Babel::UDPSocket::read(char *data, std::int64_t maxSize)
{
    return this->_socket->read(data, maxSize);
}

std::int64_t Babel::UDPSocket::write(char *data, std::int64_t size)
{
    return this->_socket->write(data, size);
}

void Babel::UDPSocket::readPending()
{
    while (this->_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = this->_socket->receiveDatagram();
        // TODO: ordre, doublon, decodage puis lecture buffer
    }
}

Babel::UDPSocket::~UDPSocket() {
    this->_socket->close();
}