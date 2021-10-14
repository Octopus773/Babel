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
#include "gui/QtBabelGui.hpp"
#include "gui/QtHomePage.hpp"
#include "Network/RFCCodes.hpp"
#include "gui/HomePage.hpp"

//int main()
//{
//	std::unique_ptr<Babel::IAudioManager> a = std::make_unique<Babel::PortAudio>();
//	std::unique_ptr<Babel::Opus> opus = std::make_unique<Babel::Opus>();
//	std::vector<int16_t> pcm;
//	std::vector<unsigned char> decoded;
//
//
//	a->openStream();
//	a->startStream();
//
//	decoded.reserve(4000);
//	for (long i = 0; i < (a->getRecordTime() * a->getSampleRate()) / a->getFramesPerBuffer(); i++) {
//		try {
//			std::vector<int16_t> data = a->readStream();
//			std::cout << data.size() << std::endl;
//			auto encoded = opus->encode(data.data(), decoded.data());
//			std::cout << encoded << std::endl;
//			opus->decode(decoded.data(), data.data(), encoded);
//			pcm.insert(pcm.end(), data.begin(), data.end());
//		}
//		catch (const Babel::PortAudioException &e) {
//			std::cerr << e.what();
//		}
//	}
//	std::cout << pcm.size() << std::endl;
//	a->writeStream(pcm);
//	return (EXIT_SUCCESS);
//}

int main(int argc, char *argv[])
{

	QApplication app(argc, argv);

	Babel::HomePage truc;


	//QApplication::setApplicationDisplayName(Client::tr("Fortune Client"));

	//Client client;
	//client.show();
	return app.exec();
}