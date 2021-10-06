#pragma once

#include <string>
#include "Exceptions/BabelException.hpp"

namespace Babel {
    class PortAudioException : public Exception::BabelException {
    public:
        explicit PortAudioException(const std::string &str) : BabelException(str) {};
    };
}