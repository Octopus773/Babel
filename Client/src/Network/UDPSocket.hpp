//
// Created by mgl on 10/11/21.
//

#ifndef BABEL_UDPSOCKET_HPP
#define BABEL_UDPSOCKET_HPP

#include <QUdpSocket>
#include "Audio/Opus/Opus.hpp"
namespace Babel {
    //! @class wrappers around Qt UDP socket
    class UDPSocket : public QObject {
    Q_OBJECT
    public:
        //! @brief ctor
        UDPSocket(std::string address, std::int16_t port, std::shared_ptr<Babel::Opus> opus, std::mutex &mtx);

        //! @brief dtor
        ~UDPSocket() override;

        //! @brief reads the incoming datagrams when they arrive
        void readPending();

        //! @brief reads maxSize from socket and writes to data
        std::int64_t read(const std::shared_ptr<char>& data, std::int64_t maxSize);

        //! @brief writes data to socket
        std::int64_t write(std::array<unsigned char, 4000> &data, const std::string &address, int port);

        //! @brief closes the socket
        void close();

    private:
        std::unique_ptr<QUdpSocket> _socket;
        std::shared_ptr<Babel::Opus> _opus;
        std::mutex &_mtx;
        std::string _address;
        std::int16_t _port;
    };
}


#endif //BABEL_UDPSOCKET_HPP
