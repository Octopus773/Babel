//
// Created by mgl on 10/13/21.
//

#ifndef BABEL_PACKET_HPP
#define BABEL_PACKET_HPP

#include <array>

namespace Babel {
    #pragma pack(push, 1)
    struct __attribute__((__packed__)) AudioPacket {

        explicit AudioPacket(std::array<unsigned char, 4000> &payloadArray) {
            std::memcpy(this->data, payloadArray.data(), payloadArray.size());
            std::uint64_t milliseconds_since_epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
            this->timestamp = milliseconds_since_epoch;
        }

        std::uint64_t timestamp;
        unsigned char data[4000] {0};
    };
    #pragma pack(pop)
}

#endif //BABEL_PACKET_HPP
