//
// Created by mgl on 10/11/21.
//

#include <QNetworkDatagram>
#include <utility>
#include "UDPSocket.hpp"
#include "NetworkException.hpp"

Babel::UDPSocket::UDPSocket(std::string address, std::int16_t port) : _address(std::move(address)), _port(port)
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

std::int64_t Babel::UDPSocket::write(const std::shared_ptr<char>& data, std::int64_t size)
{
    return this->_socket->write(data.get(), size);
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

void Babel::UDPSocket::close() {
    _socket->close();
}
