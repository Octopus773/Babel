//
// Created by cbihan on 19/09/2021.
//

#include "Audio/PortAudio.hpp"
#include "Audio/Opus/Opus.hpp"
#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<Babel::IAudioManager> a = std::make_unique<Babel::PortAudio>();
    std::unique_ptr<Babel::Opus> opus = std::make_unique<Babel::Opus>();
    std::vector<int16_t> pcm;
    std::vector<unsigned char> decoded;

    a->openStream();
    a->startStream();

    decoded.reserve(4000);
    for (long i = 0; i < (a->getRecordTime() * a->getSampleRate()) / a->getFramesPerBuffer(); i++) {
        std::vector<int16_t> data = a->readStream();
        std::cout << data.size() << std::endl;
        auto encoded = opus->encode(data.data(), decoded.data());
        std::cout << encoded << std::endl;
        opus->decode(decoded.data(), data.data(), encoded);
        pcm.insert(pcm.end(), data.begin(), data.end());
    }
    a->writeStream(pcm);
    return (EXIT_SUCCESS);
}