//
// Created by mgl on 10/13/21.
//

#ifndef BABEL_PACKET_HPP
#define BABEL_PACKET_HPP

namespace Babel {
    struct AudioPacket {
        std::uint64_t timestamp {};
        std::int32_t size {};
        unsigned char data[4000] {0};
    };
}

#endif //BABEL_PACKET_HPP
