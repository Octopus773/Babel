//
// Created by mgl on 10/11/21.
//

#ifndef BABEL_UDPSOCKET_HPP
#define BABEL_UDPSOCKET_HPP

#include <QUdpSocket>

namespace Babel {
    class UDPSocket : public QObject {
    Q_OBJECT
    public:
        UDPSocket(const QHostAddress &address, std::int16_t port);

        ~UDPSocket() override;

        void readPending();

        std::int64_t read(char *data, std::int64_t maxSize);

        std::int64_t write(char *data, std::int64_t size);

    private:
        std::unique_ptr<QUdpSocket> _socket;
        QHostAddress _address;
        std::int16_t _port;
    };
}


#endif //BABEL_UDPSOCKET_HPP
