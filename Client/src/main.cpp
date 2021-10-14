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


void audio_reception(const std::shared_ptr<Babel::PortAudio> portAudio, std::mutex &paMtx, const std::shared_ptr<Babel::Opus> opus, std::mutex &opusMtx)
{
    auto udpSock = std::make_unique<Babel::UDPSocket>("127.0.0.1", 25565, opus, opusMtx);
}

void audio_record(const std::shared_ptr<Babel::PortAudio> portAudio, std::mutex &paMtx, const std::shared_ptr<Babel::Opus> opus, std::mutex &opusMtx)
{
    std::vector<unsigned char> decoded;
    std::vector<int16_t> pcm;

    decoded.reserve(4000);
    for (long i = 0; i < (portAudio->getRecordTime() * portAudio->getSampleRate()) / portAudio->getFramesPerBuffer(); i++) {
        try {
            paMtx.lock();
            std::vector<int16_t> data = portAudio->readStream();
            paMtx.unlock();
            opusMtx.lock();
            auto encodedSize = opus->encode(data.data(), decoded.data());
            opusMtx.unlock();
            // TODO: utilsier writeDatagram
        }
        catch (const Babel::PortAudioException &e) {
            std::cerr << e.what();
        }
    }
}

int main()
{
	auto portAudio = std::make_unique<Babel::PortAudio>();
	auto opus = std::make_shared<Babel::Opus>();
    std::mutex opusMtx;
    std::mutex paMtx;

    portAudio->openStream();
	portAudio->startStream();

    //std::thread audioReceptionThread(audio_reception, portAudio, std::ref(paMtx), opus, std::ref(opusMtx));
    std::thread audioSendThread(audio_record, portAudio, std::ref(paMtx), opus, std::ref(opusMtx));

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