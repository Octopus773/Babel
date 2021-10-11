//
// Created by mgl on 10/11/21.
//

#ifndef BABEL_UDPSOCKET_HPP
#define BABEL_UDPSOCKET_HPP

#include <QUdpSocket>

namespace Babel {
    //! @class wrappers around Qt UDP socket
    class UDPSocket : public QObject {
    Q_OBJECT
    public:
        //! @brief ctor
        UDPSocket(const QHostAddress &address, std::int16_t port);

        //! @brief dtor
        ~UDPSocket() override;

        //! @brief reads the incoming datagrams when they arrive
        void readPending();

        //! @brief reads maxSize from socket and writes to data
        std::int64_t read(char *data, std::int64_t maxSize);

        //! @brief writes data to socket
        std::int64_t write(char *data, std::int64_t size);

    private:
        std::unique_ptr<QUdpSocket> _socket;
        QHostAddress _address;
        std::int16_t _port;
    };
}


#endif //BABEL_UDPSOCKET_HPP
