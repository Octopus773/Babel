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
#include "SoundHandler.hpp"

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


                // on déballe
                //auto *packetRecu = reinterpret_cast<Babel::AudioPacket *> (toSend);
                //std::uint64_t timestamp = packetRecu->timestamp;
                //std::int32_t sizeRecu = packetRecu->size;
                //std::vector<unsigned char> encodedReceived(sizeRecu);
                //std::memcpy(encodedReceived.data(), packetRecu->data, sizeRecu);
                //std::cout << "Timestamp = " << timestamp << " & size = " << sizeRecu << std::endl;


                // on décode
                //std::vector<std::int16_t> decodedData(portAudio->getFramesPerBuffer() * portAudio->getInputChannelsNumber(), 0);
                //opus->decode(encodedReceived.data(), decodedData.data(), sizeRecu);

                //portAudio->writeStream(decodedData);

                 
                // Envoi sur network
                udpSocket->write(encoded, sizeSent ,"127.0.0.1", 25565);
            } catch (const Babel::PortAudioException &e) {
                //std::cerr << e.what();
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
int main(int argc, char **argv)
{
    try
    {
        QApplication app(argc, argv);
        Babel::SoundHandler sound(25565);
        sound.addClient("Moi", "127.0.0.1", 25565);
        sound.startCall();
        return QApplication::exec();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        return (84);
    }

    return QApplication::exec();
}
 */


/*
int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	QApplication::setApplicationDisplayName(Client::tr("Fortune Client"));

	Client client;
	client.show();
	return app.exec();
}*/