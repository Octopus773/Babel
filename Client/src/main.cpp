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

/*
int main()
{
	std::unique_ptr<Babel::IAudioManager> a = std::make_unique<Babel::PortAudio>();
	std::unique_ptr<Babel::Opus> opus = std::make_unique<Babel::Opus>();
	std::vector<int16_t> pcm;
	std::vector<unsigned char> decoded;


	a->openStream();
	a->startStream();

    std::cout << "NbInputChanel: " << a->getInputChannelsNumber() << ", NbOutputChannel: " << a->getOutputChannelsNumber() << std::endl;
	decoded.reserve(4000);
	for (long i = 0; i < (a->getRecordTime() * a->getSampleRate()) / a->getFramesPerBuffer(); i++) {
		try {
			std::vector<int16_t> data = a->readStream();
            std::cout << data.size() << std::endl;
			auto encoded = opus->encode(data.data(), decoded.data());
			std::cout << encoded << std::endl;
            std::vector<std::int16_t> decodedData(a->getFramesPerBuffer() * a->getInputChannelsNumber(), 0);
			auto decodedSamples = opus->decode(decoded.data(), decodedData.data(), encoded);
            std::cout << "decoded sample = " << decodedSamples << " and data size = " << data.size() << " decodedData size = " << decodedData.size() << std::endl;
            //pcm.insert(pcm.end(), data.begin(), data.end());
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
*/

int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	QApplication::setApplicationDisplayName(Client::tr("Fortune Client"));

	Client client;
	client.show();
	return app.exec();
}