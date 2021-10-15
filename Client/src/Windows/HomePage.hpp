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

		Ui_babelHome _ui{};

		QtTCPConnection connection;

		TSQueue<RFCCodes> _requestsMade;

		//! @brief send the message to the server and add it to the message send queue to handle responses type
		//! @note The client should always use this function to send messages to the server
		void sendHandler(const Message<RFCCodes> &m);

		void onMessage(Message<RFCCodes> m);

		void doConnect();

		void doLogin();

		void doListUsers();

		void doCallUser();

		void doHangUp();

		//! @brief handler for any responses of the login request
		void onLoginResponse(const Message<RFCCodes> &m);

		//! @brief handler for any responses of the listuser request
		void onListUsersResponse(const Message<RFCCodes> &m);

		//! @brief handler for any responses of the callUser request
		void onCallUserResponse(const Message<RFCCodes> &m);

		//! @brief handler for any responses of the joinCall request
		void onJoinCall(const Message<RFCCodes> &m);

		//! @brief handler for any simple responses (opens a dialog if error) otherwise do nothing
		void onBasicResponse(const Message<RFCCodes> &m);

		//! @brief Update qt interface to display the correct username and infos
		void updateDisplaySelectedUser();

		//! @brief Change the currently selected tab
		inline void changeCurrentUITab(const std::string &tabName) {
			this->_ui.tab_handler->setCurrentWidget(this->_ui.tab_handler->findChild<QWidget *>(QString::fromStdString(tabName)));
		};


		//! @brief All the necessary information to handle a response
		struct MessageHandler
		{
			//! @brief the function to handle the request response type
			std::function<void(const Message<RFCCodes> &m)> method;
		};

		//! @brief Used to call the correct handling function
		//! @note Expects a response for each type of request, expect FIFO order
		std::map<RFCCodes, MessageHandler> _messageHandlers;

		//! @brief All the necessary information for a user in call or in the display list
		//! @note by default address is empty and port is set to 0
		struct UserInfo
		{
			//! @brief The udp address to send our voice to this user
			std::string address;
			//! @brief The udp port to send our voice to this user
			uint16_t port;
			//! @brief Tell the client if he can call this user
			bool canBeCalled;

			//! @brief Utility ctor
			explicit UserInfo(bool canBeCalled);

			//! @brief needed ctor without args
			explicit UserInfo();
		};

		//! @brief Userinfo holder the username is the key
		std::map<std::string, UserInfo> _usersInfos;

		std::list<std::string> _usersInCurrentCall;

		static constexpr int CurrentlyNotInCall = -1;

		int _currentCallId;

	};
}


