//
// Created by mgl on 10/13/21.
//

#ifndef BABEL_PACKET_HPP
#define BABEL_PACKET_HPP

#include <array>

namespace Babel {
    using namespace std::chrono;

    struct AudioPacket {
        qint32 timestamp;
        qint32 size;
        QByteArray data;
    };

}

#endif //BABEL_PACKET_HPP
