//
// Created by cbihan on 19/09/2021.
//

#include "Audio/PortAudioException.hpp"
#include <memory>
#include <QApplication>
#include "Network/QtTCPConnection.hpp"
#include "Windows/HomePage.hpp"

int main(int argc, char **argv) {
    try {
        QApplication app(argc, argv);
        Babel::HomePage hP;

        return QApplication::exec();
    }
    catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }
}