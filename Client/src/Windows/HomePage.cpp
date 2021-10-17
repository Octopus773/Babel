//
// Created by cbihan on 14/10/2021.
//

#include "HomePage.hpp"
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"
#include <iostream>
#include <memory>
#include <QMainWindow>
#include "Utilities/Utilities.hpp"
#include <QPushButton>
#include <QNetworkInterface>
#include "SoundHandler.hpp"

namespace Babel {

    HomePage::HomePage()
            : _window(new QMainWindow()),
              _audio(2456),
              _messageHandlers({
                                       {RFCCodes::Login,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->onLoginResponse(m);
                                               }}
                                       },
                                       {RFCCodes::ListUsers,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->onListUsersResponse(m);
                                               }}
                                       },
                                       {RFCCodes::CallUser,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->onCallUserResponse(m);
                                               }}
                                       },
                                       {RFCCodes::JoinCall,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->onJoinCall(m);
                                               }}
                                       },
                                       {RFCCodes::HangUp,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->onBasicResponse(m);
                                               }}
                                       },
                                       {RFCCodes::DenyCall,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->onBasicResponse(m);
                                               }}
                                       },
                                       {RFCCodes::Called,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->handleIncomingCall(m);
                                               }}
                                       },
                                       {RFCCodes::UserJoinedCall,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->handleUserJoined(m);
                                               }}
                                       },
                                       {RFCCodes::UserLeftCall,
                                               MessageHandler{[this](const Message<RFCCodes> &m) {
                                                   this->handleUserLeft(m);
                                               }}
                                       }
                               }),
              _currentCallId(CurrentlyNotInCall) {
        this->_ui.setupUi(this->_window);

        this->connection.setCallbackOnMessage([this](Message<RFCCodes> m) {
            this->onMessage(std::move(m));
        });

        QObject::connect(this->_ui.button_connect, &QPushButton::clicked, this, &HomePage::doConnect);
        QObject::connect(this->_ui.button_login, &QPushButton::clicked, this, &HomePage::doLogin);

        QObject::connect(this->_ui.output_connected_user_list, &QListWidget::currentItemChanged, this,
                         &HomePage::updateDisplaySelectedUser);
        QObject::connect(this->_ui.button_call_user, &QPushButton::clicked, this, &HomePage::doCallUser);
        QObject::connect(this->_ui.button_hang_up, &QPushButton::clicked, this, &HomePage::doHangUp);
        QObject::connect(this->_ui.button_refresh_connected_user_list, &QPushButton::clicked, this,
                         &HomePage::doListUsers);

        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
                this->_address = address.toString().toStdString();
        }

        this->_window->show();
        this->_ui.page_login->setDisabled(true);
        this->_ui.page_call->setDisabled(true);
        this->_ui.page_server->setDisabled(true);
    }

    void HomePage::doConnect() {
        this->connection.connect(this->_ui.input_address->text().toStdString(), this->_ui.input_port->value());
        this->_ui.page_login->setDisabled(false);
    }

    void HomePage::doLogin() {
        Message<RFCCodes> m;
        m.header.codeId = RFCCodes::Login;
        const std::string &username = this->_ui.input_login_username->text().toStdString();
        m << static_cast<uint8_t>(username.size()) << username;
        if (!username.empty()) {
            this->_username = username;
            this->sendHandler(m);
        }
    }

    void HomePage::onMessage(Message<RFCCodes> m) {
        RFCCodes requestType;
        if (m.header.codeId == RFCCodes::Called
            || m.header.codeId == RFCCodes::UserJoinedCall
            || m.header.codeId == RFCCodes::UserLeftCall) {
            std::cout << "receiving event: header codeId: " << static_cast<uint16_t>(m.header.codeId) << std::endl;
            requestType = m.header.codeId;
        } else if (this->_requestsMade.empty()) {
            std::cout << "receiving response but no request left" << std::endl;
            return;
        } else {
            requestType = this->_requestsMade.popFront();
        }

        if (!this->_messageHandlers.contains(requestType)) {
            std::cout << "no handler for this message type: " << static_cast<uint16_t>(requestType) << std::endl;
            return;
        }
        this->_messageHandlers[requestType].method(m);
    }

    void HomePage::onLoginResponse(const Message<RFCCodes> &m) {
        uint16_t codeId;
        uint8_t descLength;

        Message<RFCCodes> message(m);

        message >> codeId >> descLength;

        std::string desc;
        Message<RFCCodes>::GetBytes(message, desc, descLength);

        std::cout << "CodeId: " << codeId << " desc: " << desc << std::endl;

        if (codeId != 1) {
            QMessageBox::information(nullptr, tr("Babel"),
                                     tr(desc.data()));
            this->_username = "";
            return;
        }
        this->_ui.page_server->setDisabled(false);
        this->doListUsers();
        this->changeCurrentUITab("page_server");
    }

    void HomePage::sendHandler(const Message<RFCCodes> &m) {
        this->_requestsMade.pushBack(m.header.codeId);
        this->connection.send(m);
    }

    void HomePage::onListUsersResponse(const Message<RFCCodes> &m) {
        uint16_t codeId;
        Message<RFCCodes> message(m);

        message >> codeId;
        if (codeId != 1) {
            std::cout << "error in list users" << std::endl;
            return;
        }

        this->_ui.output_connected_user_list->blockSignals(true);
        this->_ui.output_connected_user_list->clearSelection();
        this->_ui.output_connected_user_list->clearFocus();
        this->_ui.output_connected_user_list->clear();
        this->_ui.output_connected_user_list->blockSignals(false);

        uint16_t arrayLength;
        message >> arrayLength;

        for (uint16_t i = 0; i < arrayLength; i++) {
            std::string username;
            if (!Utils::getString(message, username, {3, 10})) {
                std::cout << "error while reading " << i << " user" << std::endl;
            }
            uint8_t canBeCalled;
            message >> canBeCalled;

            this->_usersInfos[username] = UserInfo{static_cast<bool>(canBeCalled)};
            this->_ui.output_connected_user_list->addItem(QString::fromStdString(username));
        }
    }

    void HomePage::doListUsers() {
        Message<RFCCodes> m;
        m.header.codeId = RFCCodes::ListUsers;
        this->sendHandler(m);
    }

    void HomePage::updateDisplaySelectedUser() {
        QString value = this->_ui.output_connected_user_list->currentItem()->text();

        UserInfo &userInfo = this->_usersInfos[value.toStdString()];

        this->_ui.output_selected_username->setText(value);
        this->_ui.output_can_be_called->setChecked(userInfo.canBeCalled);
        this->_ui.button_call_user->setEnabled(userInfo.canBeCalled);
    }

    void HomePage::doCallUser() {
        if (this->_currentCallId != CurrentlyNotInCall) {
            QMessageBox::information(nullptr, tr("Babel"),
                                     tr("You must leave your call before starting another one."));
            return;
        }

        QString value = this->_ui.output_connected_user_list->currentItem()->text();
        std::string usernameToCall = value.toStdString();

        if (!this->_usersInfos.contains(usernameToCall)) {
            std::cout << "doCallUser: no corresponding user" << std::endl;
            return;
        }
        Message<RFCCodes> m;

        m.header.codeId = RFCCodes::CallUser;
        m << static_cast<uint8_t>(usernameToCall.size()) << usernameToCall;


        this->sendHandler(m);
    }

    void HomePage::onCallUserResponse(const Message<RFCCodes> &m) {
        uint16_t codeId;

        Message<RFCCodes> message(m);
        message >> codeId;

        std::string desc;
        Utils::getString(message, desc);

        if (codeId != 1) {
            std::cout << "error: callUser " << desc << std::endl;
            return;
        }

        uint16_t callId;
        if (!Utils::tryParse(desc, callId)) {
            std::cout << "error: callUser callId" << std::endl;
            return;
        }

        message.reset();
        message.header.codeId = RFCCodes::JoinCall;
        message << callId;

        std::string address = this->_address;
        uint16_t port = 2456;
        this->doJoinCall(callId, address, port);
    }

    void HomePage::onJoinCall(const Message<RFCCodes> &m) {
        Message<RFCCodes> message(m);
        uint16_t codeId;

        message >> codeId;

        if (codeId != 1) {
            std::string desc;
            Utils::getString(message, desc);
            std::cout << "error: onJoinCall: " << desc << std::endl;
            this->_currentCallId = CurrentlyNotInCall;
            return;
        }

        uint16_t arrayLength;

        message >> arrayLength;

        for (uint16_t i = 0; i < arrayLength; i++) {
            std::string address;
            Utils::getString(message, address);
            uint16_t port;
            message >> port;
            std::string username;
            Utils::getString(message, username);

            this->_usersInfos[username].port = port;
            this->_usersInfos[username].address = address;
            this->_usersInCurrentCall.emplace_back(username);
            this->_ui.output_list_call_members->addItem(QString::fromStdString(username));
            if (username != this->_username)
                this->_audio.addClient(username, address, port);
        }

        this->_ui.page_call->setDisabled(false);
        this->changeCurrentUITab("page_call");
        // todo send audio packets to every address and port
        this->_audio.startCall();
        // switch to call tab
    }

    void HomePage::doHangUp() {
        if (this->_currentCallId == CurrentlyNotInCall) {
            std::cout << "no callid in hangup" << std::endl;
            return;
        }
        Message<RFCCodes> m;

        m.header.codeId = RFCCodes::HangUp;
        m << static_cast<uint16_t>(this->_currentCallId);

        // todo close all udp sockets from _usersInCurrentCall
        this->_audio.stopCall();


        this->_ui.output_list_call_members->blockSignals(true);
        this->_ui.output_list_call_members->clearSelection();
        this->_ui.output_list_call_members->clearFocus();
        this->_ui.output_list_call_members->clear();
        this->_ui.output_list_call_members->blockSignals(false);


        this->_usersInCurrentCall.clear();
        this->_currentCallId = CurrentlyNotInCall;
        this->sendHandler(m);
        this->_ui.page_call->setDisabled(true);
        this->changeCurrentUITab("page_server");
    }

    void HomePage::onBasicResponse(const Message<RFCCodes> &m) {
        uint16_t codeId;
        Message<RFCCodes> message(m);

        message >> codeId;

        if (codeId != 1) {
            std::string desc;
            Utils::getString(message, desc);
            QMessageBox::information(nullptr, tr("Babel"), tr(desc.data()));
        }
    }

    void HomePage::doDenyCall(int callId) {
        Message<RFCCodes> m;
        m.header.codeId = RFCCodes::DenyCall;
        m << static_cast<uint16_t>(callId);

        this->sendHandler(m);
    }

    void HomePage::doJoinCall(int callId, std::string &address, uint16_t port) {
        Message<RFCCodes> m;
        this->_currentCallId = callId;
        m.header.codeId = RFCCodes::JoinCall;
        m << static_cast<uint16_t>(callId);
        m << static_cast<uint8_t>(address.size()) << address << port;

        this->sendHandler(m);
    }

    void HomePage::handleIncomingCall(const Message<RFCCodes> &m) {
        Message<RFCCodes> message(m);

        uint16_t callId;
        message >> callId;
        std::string invitator;
        Utils::getString(message, invitator);


        QMessageBox msgBox;
        msgBox.setText(
                QString::fromStdString("You're receiving an incoming from " + invitator + "\nDo you accept it ?"));
        msgBox.setWindowTitle("Babel");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Question);
        switch (msgBox.exec()) {
            case QMessageBox::Yes:
                this->doJoinCall(callId, this->_address, 2456);
                break;
            case QMessageBox::No:
            default:
                this->doDenyCall(callId);
                break;
        }

    }

    void HomePage::handleUserJoined(const Message<RFCCodes> &m) {
        if (this->_currentCallId == CurrentlyNotInCall) {
            std::cout << "no call id in user joined" << std::endl;
            return;
        }
        Message<RFCCodes> message(m);
        std::string address;
        Utils::getString(message, address);
        uint16_t port;
        message >> port;
        std::string username;
        Utils::getString(message, username);

        this->_usersInfos[username].address = address;
        this->_usersInfos[username].port = port;
        this->_usersInfos[username].canBeCalled = false;

        this->_usersInCurrentCall.push_back(username);

        this->_ui.output_list_call_members->addItem(QString::fromStdString(username));

        // todo send our udp data to his socket
        std::cout << port << std::endl;
        this->_audio.addClient(username, address, port);
    }

    void HomePage::handleUserLeft(const Message<RFCCodes> &m) {
        if (this->_currentCallId == CurrentlyNotInCall) {
            std::cout << "no call id in user left" << std::endl;
            return;
        }
        Message<RFCCodes> message(m);

        std::string username;
        Utils::getString(message, username);

        this->_audio.removeClient(username);

        this->_usersInCurrentCall.erase(
                std::remove(this->_usersInCurrentCall.begin(), this->_usersInCurrentCall.end(), username),
                this->_usersInCurrentCall.end());

        auto namesToRemove = this->_ui.output_list_call_members->findItems(QString::fromStdString(username),
                                                                           Qt::MatchExactly);
        for (auto &name: namesToRemove) {
            this->_ui.output_list_call_members->takeItem(this->_ui.output_list_call_members->row(name));
            delete name;
        }
    }

    HomePage::UserInfo::UserInfo(bool cBC)
            : address(),
              port(0),
              canBeCalled(cBC) {
    }

    HomePage::UserInfo::UserInfo()
            : UserInfo(false) {
    }
}