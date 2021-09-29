//
// Created by mgl on 9/29/21.
//

#ifndef BABEL_OPUSEXCEPTION_HPP
#define BABEL_OPUSEXCEPTION_HPP

#include <string>
#include "Exceptions/BabelException.hpp"

namespace Babel {
    class OpusException : public BabelException {
    public:
        explicit OpusException(const std::string &str) : BabelException(str) {};
    };
}

#endif //BABEL_OPUSEXCEPTION_HPP
