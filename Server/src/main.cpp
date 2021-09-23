//
// Created by cbihan on 19/09/2021.
//

#include <iostream>
#include <portaudio.h>
#include "Exceptions/BabelException.hpp"

int main()
{
	std::cout << "Hello from server" << std::endl;
	std::cout << "pulseaudio version: " << Pa_GetVersion() << std::endl;
	throw Babel::Exception::BabelException("This is from common sources");
}