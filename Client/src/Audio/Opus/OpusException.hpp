#pragma once

#include <string>
#include "Exceptions/BabelException.hpp"

namespace Babel {
    class OpusException : public Exception::BabelException {
    public:
        explicit OpusException(const std::string &str) : BabelException(str) {};
    };
}