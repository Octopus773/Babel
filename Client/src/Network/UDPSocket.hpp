//
// Created by mgl on 10/11/21.
//

#ifndef BABEL_UDPSOCKET_HPP
#define BABEL_UDPSOCKET_HPP

#include <QUdpSocket>
#include "Audio/Opus/ICodec.hpp"
#include "Audio/IAudioManager.hpp"

namespace Babel {
    //! @class wrappers around Qt UDP socket
    class UDPSocket : public QObject {
    Q_OBJECT
    public:
        //! @brief ctor
        UDPSocket(const std::string &address, std::int16_t port, std::shared_ptr<Babel::IAudioManager>, std::shared_ptr<Babel::ICodec>);

        //! @brief dtor
        ~UDPSocket() override;

        //! @brief writes data to socket
        std::int64_t write(std::array<unsigned char, 4000> &data, const std::string &address, int port);

        //! @brief closes the socket
        void close();

        //! @brief reads the incoming datagrams when they arrive
        void readPending();

    private:
        std::unique_ptr<QUdpSocket> _socket;
        std::mutex _mutex;
        std::shared_ptr<Babel::IAudioManager> _audio;
        std::shared_ptr<Babel::ICodec> _codec;
        std::string _address;
        std::int16_t _port;
    };
}


#endif //BABEL_UDPSOCKET_HPP
