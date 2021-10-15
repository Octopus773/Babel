//
// Created by cbihan on 14/10/2021.
//

#pragma once

#include "ui/UIHomePage.hpp"
#include "ui/UiLogin.hpp"
#include "Network/QtTCPConnection.hpp"
#include <QMainWindow>
#include <functional>
#include <map>
#include <QObject>

namespace Babel
{

	class HomePage : public QObject
	{
	Q_OBJECT
	public:
		HomePage();

		QMainWindow *_window;

		QMainWindow *_windowLogin;

		Ui_babelHome _ui{};

		Ui_Login _uiLogin{};

		QtTCPConnection connection;

		TSQueue<RFCCodes> _requestsMade;

		//! @brief send the message to the server and add it to the message send queue to handle responses type
		//! @note The client should always use this function to send messages to the server
		void sendHandler(const Message<RFCCodes> &m);

		void onMessage(Message<RFCCodes> m);

		void doConnect();

		void doLogin();

		void doListUsers();

		//! @brief handler for any responses of the login request
		void onLoginResponse(const Message<RFCCodes> &m);

		//! @brief handler for any responses of the listuser request
		void onListUsersResponse(const Message<RFCCodes> &m);

		//! @brief Update qt interface to display the correct username and infos
		void updateDisplaySelectedUser();


		struct MessageHandler
		{
			std::function<void(const Message<RFCCodes> &m)> method;
		};

		std::map<RFCCodes, MessageHandler> _messageHandlers;

		struct UserInfo
		{
			bool canBeCalled;
		};

		std::map<std::string, UserInfo> _usersInfos;
	};
}


