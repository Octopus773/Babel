//
// Created by cbihan on 14/10/2021.
//

#pragma once

#include "ui/UIHomePage.hpp"
#include "Network/QtTCPConnection.hpp"
#include <QMainWindow>
#include <functional>
#include <map>
#include <QObject>
#include "SoundHandler.hpp"

namespace Babel
{
	//! @brief Main client Class
	class HomePage : public QObject
	{
	Q_OBJECT
	public:
		//! @brief ctor
		HomePage();

		//! @brief dtor
		~HomePage() override= default;

		//! @brief The port to receive udp audio packets
		const uint16_t udpPort = 23476;

	private:
		//! @brief The display window
		QMainWindow *_window;

		//! @brief The UI Class
		Ui_babelHome _ui{};

		//! @brief tcp connection handler
		QtTCPConnection _connection;

		//! @brief all the request made by the client (expecting a response)
		TSQueue<RFCCodes> _requestsMade;

		//! @brief Audio call manager
		SoundHandler _audio;

		//! @brief send the message to the server and add it to the message send queue to handle responses type
		//! @note The client should always use this function to send messages to the server
		void sendHandler(const Message<RFCCodes> &m);

		//! @brief is used to redirect responses from request to the correct handling functions
		void onMessage(Message<RFCCodes> m);

		//! @brief Connects to the server with tcp
		void doConnect();

		//! @brief send a login request, look into field inputs for username
		void doLogin();

		//! @brief send a listUsers request
		void doListUsers();

		//! @brief Call a specific user look in the list for the selected username
		void doCallUser();

		//! @brief join the callId given
		void doJoinCall(int callId, std::string &address, uint16_t port);

		//! @brief deny a callId
		void doDenyCall(int callId);

		//! @brief quit a call
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
		static void onBasicResponse(const Message<RFCCodes> &m);

		//! @brief handle incoming call request
		void handleIncomingCall(const Message<RFCCodes> &m);

		//! @brief handle user joined request
		void handleUserJoined(const Message<RFCCodes> &m);

		//! @brief handle user left request
		void handleUserLeft(const Message<RFCCodes> &m);

		//! @brief Update qt interface to display the correct username and infos
		void updateDisplaySelectedUser();

		//! @brief Change the currently selected tab
		inline void changeCurrentUITab(QWidget *tab)
		{
			this->_ui.tab_handler->setCurrentWidget(tab);
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

		//! @brief a list containing all users in the currentCall
		std::list<std::string> _usersInCurrentCall;

		//! @brief Used to set and verify the _currentCallId if a call is ongoing
		static constexpr int CurrentlyNotInCall = -1;

		//! @brief handling the callId of the current call (to be able to hang up, ...)
		//! @note when set to CurrentlyNotInCall no call is happening
		int _currentCallId;

		//! @brief The local ip
		//! @note the address to receive udp audio packets
		std::string _address;

		//! @brief The username of the connected user
		//! @note set to an empty string when not connected
		std::string _username;

	};
}


