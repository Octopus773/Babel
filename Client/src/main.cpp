//
// Created by cbihan on 19/09/2021.
//

#include "Audio/PortAudio.hpp"
#include "Audio/Opus/Opus.hpp"
#include "Audio/PortAudioException.hpp"
#include <memory>
#include <QApplication>
#include "Network/QtTCPConnection.hpp"
#include "Network/RFCCodes.hpp"
#include "Network/UDPSocket.hpp"
#include "Network/AudioPacket.hpp"

void audio_record(const std::shared_ptr<Babel::IAudioManager> portAudio, const std::shared_ptr<Babel::ICodec> opus, const std::shared_ptr<Babel::UDPSocket> udpSocket)
{
    std::array<unsigned char, 4000> encoded {0};
    std::vector<int16_t> pcm;

    while (true) {
        for (long i = 0; i < (portAudio->getRecordTime() * portAudio->getSampleRate()) / portAudio->getFramesPerBuffer(); i++) {
            try {
                // Enregistrement du son
                std::vector<int16_t> data = portAudio->readStream();

                // Compression
                opus->encode(data.data(), encoded.data());

                // Envoi sur network
                udpSocket->write(encoded, "10.29.125.137", 25565);
            } catch (const Babel::PortAudioException &e) {
                //std::cerr << e.what();
            }
        }
    }
}

int main(int argc, char **argv)
{
	std::shared_ptr<Babel::IAudioManager> portAudio = std::make_shared<Babel::PortAudio>();
	std::shared_ptr<Babel::ICodec> opus = std::make_shared<Babel::Opus>();

    QApplication app(argc, argv);

    portAudio->openStream();
	portAudio->startStream();

    std::shared_ptr<Babel::UDPSocket> udpSock = std::make_shared<Babel::UDPSocket>(25565, portAudio, opus);

    std::thread recordThread(audio_record, portAudio, opus, udpSock);

    return QApplication::exec();
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