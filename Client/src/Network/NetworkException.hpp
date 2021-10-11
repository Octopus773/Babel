#pragma once

#include <string>
#include "Exceptions/BabelException.hpp"

namespace Babel {
    class NetworkException : public Exception::BabelException {
    public:
        explicit NetworkException(const std::string &str) : BabelException(str) {};
    };
}