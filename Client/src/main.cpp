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


int main()
{
	std::unique_ptr<Babel::IAudioManager> a = std::make_unique<Babel::PortAudio>();
	std::unique_ptr<Babel::Opus> opus = std::make_unique<Babel::Opus>();
	std::vector<int16_t> pcm;
	std::vector<unsigned char> decoded;
    auto udpSock = std::make_unique<Babel::UDPSocket>("127.0.0.1", 25565);

    a->openStream();
	a->startStream();

	decoded.reserve(4000);
	for (long i = 0; i < (a->getRecordTime() * a->getSampleRate()) / a->getFramesPerBuffer(); i++) {
		try {
			std::vector<int16_t> data = a->readStream();
			auto encoded = opus->encode(data.data(), decoded.data());
            std::vector<std::int16_t> decodedData(a->getFramesPerBuffer() * a->getInputChannelsNumber(), 0);
			opus->decode(decoded.data(), decodedData.data(), encoded);
            a->writeStream(decodedData);
		}
		catch (const Babel::PortAudioException &e) {
			std::cerr << e.what();
		}
	}
	std::cout << pcm.size() << std::endl;
	a->writeStream(pcm);
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