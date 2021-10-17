//
// Created by cbihan on 19/09/2021.
//

#include "Audio/PortAudio.hpp"
#include "Audio/Opus/Opus.hpp"
#include "Audio/PortAudioException.hpp"
#include <memory>
#include <QApplication>
#include "Network/QtTCPConnection.hpp"
#include "gui/QtBabelGui.hpp"
#include "gui/QtHomePage.hpp"
#include "Network/RFCCodes.hpp"
#include "Network/UDPSocket.hpp"
#include "Network/AudioPacket.hpp"
#include "SoundHandler.hpp"
#include "Windows/HomePage.hpp"

void audio_record(const std::shared_ptr<Babel::IAudioManager> portAudio, const std::shared_ptr<Babel::ICodec> opus, const std::shared_ptr<Babel::UDPSocket> udpSocket)
{
    std::array<unsigned char, 4000> encoded {0};
    std::vector<int16_t> pcm;

    while (true) {
            try {
                // Enregistrement du son
                std::vector<int16_t> data = portAudio->readStream();

                // Compression
                std::int32_t sizeSent = opus->encode(data.data(), encoded.data());


                // on fait un paquet
                Babel::AudioPacket packetEnvoye(encoded, sizeSent);
                char toSend[sizeof(Babel::AudioPacket)];
                std::memcpy(toSend, &packetEnvoye, sizeof(packetEnvoye));        
                // Envoi sur network
                udpSocket->write(encoded, sizeSent ,"127.0.0.1", 25565);
            } catch (const Babel::PortAudioException &e) {
                //std::cerr << e.what();
            }
    }
}

/*int main(int argc, char **argv)
{
	std::shared_ptr<Babel::IAudioManager> portAudio = std::make_shared<Babel::PortAudio>();
	std::shared_ptr<Babel::ICodec> opus = std::make_shared<Babel::Opus>();

    QApplication app(argc, argv);
    Babel::HomePage a;
    portAudio->openStream();
	portAudio->startStream();

    std::shared_ptr<Babel::UDPSocket> udpSock = std::make_shared<Babel::UDPSocket>(25565, portAudio, opus);

    std::thread recordThread(audio_record, portAudio, opus, udpSock);

    return QApplication::exec();
}*/


int main(int argc, char **argv)
{
    try
    {
        QApplication app(argc, argv);
       Babel::HomePage hP;

        return QApplication::exec();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        return (84);
    }
    return (0);
}


/*
int main(int argc, char *argv[])
{

	QApplication app(argc, argv);

	Babel::HomePage truc;


	//QApplication::setApplicationDisplayName(Client::tr("Fortune Client"));

	//Client client;
	//client.show();
	return app.exec();
}*/