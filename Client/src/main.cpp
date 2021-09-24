//
// Created by cbihan on 19/09/2021.
//

#include <iostream>
#include <portaudio.h>
#include <opus/opus.h>
//#include <qt5/QtCore/QtCoreVersion>

int main()
{
	std::cout << "Hello from client" << std::endl;
	std::cout << "pulseaudio version: " << Pa_GetVersion() << std::endl;
	std::cout << "opus version " << opus_get_version_string() << std::endl;
	//std::cout << "qT version " << QTCORE_VERSION_STR << " : " << QTCORE_VERSION << std::endl;
}