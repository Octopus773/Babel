//
// Created by cbihan on 19/09/2021.
//

#include "Audio/PortAudio.hpp"
#include "Audio/Opus/Opus.hpp"
#include "Audio/PortAudioException.hpp"
#include <iostream>
#include <memory>
#include <QApplication>
#include "client.hpp"
#include "Network/QtTCPConnection.hpp"
#include "Network/RFCCodes.hpp"
#include "Network/UDPSocket.hpp"


void audio_reception(const std::shared_ptr<Babel::Opus> &opus, std::mutex &opusMtx, std::mutex &paMtx)
{
    auto udpSock = std::make_unique<Babel::UDPSocket>("127.0.0.1", 25565, opus, opusMtx);
}

void audio_record(std::mutex &paMtx)
{
    std::vector<unsigned char> decoded;
    std::vector<int16_t> pcm;

    decoded.reserve(4000);
    for (long i = 0; i < (a->getRecordTime() * a->getSampleRate()) / a->getFramesPerBuffer(); i++) {
        try {
            mtx.lock();
            std::vector<int16_t> data = a->readStream();
            auto encodedSize = opus->encode(data.data(), decoded.data());
            mtx.unlock();
        }
        catch (const Babel::PortAudioException &e) {
            std::cerr << e.what();
        }
    }
}

int main()
{
	auto a = std::make_unique<Babel::PortAudio>();
	auto opus = std::make_shared<Babel::Opus>();
    std::mutex opusMtx;
    std::mutex paMtx;

    a->openStream();
	a->startStream();

    std::thread audioThread(audio_reception, opus, std::ref(opusMtx));
    std::thread audioReadThread(audio_record, std::ref(paMtx));

	return (EXIT_SUCCESS);
}


/*
int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	QApplication::setApplicationDisplayName(Client::tr("Fortune Client"));

	Client client;
	client.show();
	return app.exec();
}
 */