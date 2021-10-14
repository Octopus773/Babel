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


void audio_reception(const std::shared_ptr<Babel::IAudioManager> portAudio, const std::shared_ptr<Babel::ICodec> codec, std::mutex &audio_mtx, std::mutex &codec_mtx)
{
    auto udpSock = std::make_unique<Babel::UDPSocket>("127.0.0.1", 25565, portAudio, codec, audio_mtx, codec_mtx);
}

void audio_record(const std::shared_ptr<Babel::IAudioManager> portAudio, std::mutex &paMtx, const std::shared_ptr<Babel::ICodec> opus, std::mutex &opusMtx)
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
	std::shared_ptr<Babel::IAudioManager> portAudio = std::make_shared<Babel::PortAudio>();
	std::shared_ptr<Babel::ICodec> opus = std::make_shared<Babel::Opus>();
    std::mutex opusMtx;
    std::mutex paMtx;

    portAudio->openStream();
	portAudio->startStream();

    std::thread audioReceptionThread(audio_reception, portAudio, opus, std::ref(paMtx), std::ref(opusMtx));
    //std::thread audioSendThread(audio_record, portAudio, std::ref(paMtx), opus, std::ref(opusMtx));

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