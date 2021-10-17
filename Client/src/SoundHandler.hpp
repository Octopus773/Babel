//
// SoundHhandler class
// @brief Handle the sound inputs and outputs, as well as the udp connexion
//

#ifndef SOUNDHANDLER_HPP_
#define SOUNDHANDLER_HPP_

#include "Audio/Opus/ICodec.hpp"
#include "Audio/IAudioManager.hpp"
#include "Audio/PortAudioException.hpp"
#include <memory>
#include <map>
#include <thread>
#include <condition_variable>
#include <QApplication>
#include "Network/QtTCPConnection.hpp"
#include "Network/RFCCodes.hpp"
#include "Network/UDPSocket.hpp"
#include "Network/AudioPacket.hpp"

namespace Babel
{
    class SoundHandler
    {
        public:
        // @brief sound handler constructor 
        SoundHandler(std::int16_t port);
        // @brief sound handler destructor
        ~SoundHandler();
        // @brief notify the sound handler to start to send data and to receive it
        void startCall(void);
        // @brief notify the sound handler to stop
        void stopCall(void);
        // @brief add a client to the current call
        void addClient(const std::string userid, const std::string ipadddres, std::uint16_t port);
        // @brief remove a client to the current call
        void removeClient(const std::string userid);
        private:
        // @brief pointer to audio input and outputs
        std::shared_ptr<Babel::IAudioManager> _audio;
        // @brief pointer to the codec
        std::shared_ptr<Babel::ICodec> _codec;
        // @brief pointer to the udp socket for input and output on network
        std::shared_ptr<Babel::UDPSocket> _socket;
        // @brief list of all users used in current call
        std::map<std::string, std::pair<std::string, std::uint16_t>> _userlist;
        // @brief mutex to access the userlist
        std::mutex _userlist_mtx;
        // @condition variable for stopping or enabling thread
        std::condition_variable _blocker; 
        // @brief boolean to tell when the thread should exit;
        bool _shouldExit;
        // @brief mutex for should exit variable
        std::mutex _exit_mtx;
        // @brief boolean to tell when the thread should record;
        bool _shouldPlay;
        // @brief mutex for should exit variable
        std::mutex _play_mtx;
        // @brief thread for broadcasting data from audio library to udp
        std::thread _execthread;
        //! @brief mutex used for the condition variable
        std::mutex _condVarMutex;
    };
}

#endif

