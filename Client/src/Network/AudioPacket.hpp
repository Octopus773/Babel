//
// Created by mgl on 10/13/21.
//

#ifndef BABEL_PACKET_HPP
#define BABEL_PACKET_HPP

#include <array>

namespace Babel {
    using namespace std::chrono;
    #pragma pack(push, 1)
    // __attribute__((__packed__))
    struct AudioPacket {

        explicit AudioPacket(std::array<unsigned char, 4000> &payloadArray, std::int32_t receivedSize) {
            std::memcpy(this->data, payloadArray.data(), receivedSize);
            milliseconds ms = duration_cast< milliseconds >(
                    system_clock::now().time_since_epoch()
            );
            this->timestamp = ms.count() % 1000;
            this->size = receivedSize;
        }

        std::uint64_t timestamp;
        std::int32_t size;
        unsigned char data[4000] {0};
    };
    #pragma pack(pop)

}

#endif //BABEL_PACKET_HPP
