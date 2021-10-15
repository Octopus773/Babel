//
// Created by cbihan on 14/10/2021.
//

#include "HomePage.hpp"
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"
#include <iostream>
#include <QMainWindow>
#include "Utilities/Utilities.hpp"
#include <QPushButton>


namespace Babel
{

	HomePage::HomePage()
		: _window(new QMainWindow()),

		  _windowLogin(new QMainWindow()),
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
			                   }
		  })
	{
		this->_ui.setupUi(this->_window);
		this->_uiLogin.setupUi(this->_windowLogin);

		this->connection.setCallbackOnMessage([this](Message<RFCCodes> m) {
			this->onMessage(std::move(m));
		});

		QObject::connect(this->_ui.button_connect, &QPushButton::clicked, this, &HomePage::doConnect);
		QObject::connect(this->_ui.button_login, &QPushButton::clicked, this, &HomePage::doLogin);

		QObject::connect(this->_ui.output_connected_user_list, &QListWidget::currentItemChanged, this, &HomePage::updateDisplaySelectedUser);

		this->_window->show();
		//this->_windowLogin->show();
		this->_ui.page2->setDisabled(true);
	}

	void HomePage::doConnect()
	{
		this->connection.connect(this->_ui.input_address->text().toStdString(), this->_ui.input_port->value());
		this->_ui.page2->setDisabled(false);
	}

	void HomePage::doLogin()
	{
		Message<RFCCodes> m;
		m.header.codeId = RFCCodes::Login;
		const std::string &username = this->_ui.input_login_username->text().toStdString();
		m << static_cast<uint8_t>(username.size()) << username;
		if (!username.empty()) {
			this->sendHandler(m);
		}
	}

	void HomePage::onMessage(Message<RFCCodes> m)
	{
		if (this->_requestsMade.empty()) {
			std::cout << "receiving event" << std::endl;
			return;
		}

		const RFCCodes &requestType = this->_requestsMade.popFront();
		if (!this->_messageHandlers.contains(requestType)) {
			std::cout << "no handler for this message type: " << static_cast<uint16_t>(requestType) << std::endl;
			return;
		}
		this->_messageHandlers[requestType].method(m);
	}

	void HomePage::onLoginResponse(const Message<RFCCodes> &m)
	{
		uint16_t codeId;
		uint8_t descLength;

		Message<RFCCodes> message(m);

		message >> codeId >> descLength;

		std::string desc;
		Message<RFCCodes>::GetBytes(message, desc, descLength);

		std::cout << "CodeId: " << codeId << " desc: " << desc << std::endl;

		if (codeId) {
			this->_ui.page2->setDisabled(false);
			this->doListUsers();
		}
	}

	void HomePage::sendHandler(const Message<RFCCodes> &m)
	{
		this->_requestsMade.pushBack(m.header.codeId);
		this->connection.send(m);
	}

	void HomePage::onListUsersResponse(const Message<RFCCodes> &m)
	{
		uint16_t codeId;
		Message<RFCCodes> message(m);

		message >> codeId;
		if (codeId != 1) {
			std::cout << "error in list users" << std::endl;
			return;
		}

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

	void HomePage::doListUsers()
	{
		Message<RFCCodes> m;
		m.header.codeId = RFCCodes::ListUsers;
		this->sendHandler(m);
	}

	void HomePage::updateDisplaySelectedUser()
	{
		QString value = this->_ui.output_connected_user_list->currentItem()->text();

		UserInfo &userInfo = this->_usersInfos[value.toStdString()];

		this->_ui.output_selected_username->setText(value);
		this->_ui.output_can_be_called->setChecked(userInfo.canBeCalled);
		this->_ui.button_call_user->setEnabled(userInfo.canBeCalled);
	}
}