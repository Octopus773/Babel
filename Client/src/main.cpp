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
#include "Network/AudioPacket.hpp"

void audio_record(const std::shared_ptr<Babel::IAudioManager> portAudio, std::mutex &paMtx, const std::shared_ptr<Babel::ICodec> opus, std::mutex &opusMtx, const std::shared_ptr<Babel::UDPSocket> udpSocket, std::mutex &udpMtx)
{
    std::array<unsigned char, 4000> encoded {0};
    std::vector<int16_t> pcm;

    while (1) {
        for (long i = 0; i < (portAudio->getRecordTime() * portAudio->getSampleRate()) / portAudio->getFramesPerBuffer(); i++) {
            try {
                paMtx.lock();
                std::vector<int16_t> data = portAudio->readStream();
                paMtx.unlock();
                //opusMtx.lock();
                opus->encode(data.data(), encoded.data());
                //opusMtx.unlock();
                //udpMtx.lock();
                //std::cout << "Sending packet" << std::endl;
                udpSocket->write(encoded, "10.29.125.231", 25565);
                //udpMtx.unlock();
            }
            catch (const Babel::PortAudioException &e) {
                //std::cerr << e.what();
            }
        }
    }
}

int main()
{
	std::shared_ptr<Babel::IAudioManager> portAudio = std::make_shared<Babel::PortAudio>();
	std::shared_ptr<Babel::ICodec> opus = std::make_shared<Babel::Opus>();
    std::mutex opusMtx;
    std::mutex paMtx;
    std::mutex udpMtx;
    std::string address;

    portAudio->openStream();
	portAudio->startStream();

    std::shared_ptr<Babel::UDPSocket> udpSock = std::make_shared<Babel::UDPSocket>("127.0.0.1", 25565, portAudio,
                                                                                       opus, paMtx, opusMtx, udpMtx);

    std::thread audioSendThread(audio_record, portAudio, std::ref(paMtx), opus, std::ref(opusMtx), udpSock,
                                    std::ref(udpMtx));

    audioSendThread.join();

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