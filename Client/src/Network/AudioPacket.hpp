//
// Created by mgl on 10/13/21.
//

#ifndef BABEL_PACKET_HPP
#define BABEL_PACKET_HPP

namespace Babel {
    struct AudioPacket {
        std::int32_t timestamp;
        unsigned char data[4000];
    };
}

#endif //BABEL_PACKET_HPP
