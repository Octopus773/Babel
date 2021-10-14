//
// Created by cbihan on 14/10/2021.
//

#include "HomePage.hpp"
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"
#include <iostream>
#include <QMainWindow>
#include <QPushButton>


namespace Babel
{

	HomePage::HomePage()
		: _window(new QMainWindow()),
		  _windowLogin(new QMainWindow())
	{
		this->_ui.setupUi(this->_window);
		this->_uiLogin.setupUi(this->_windowLogin);

		this->connection.setCallbackOnMessage([this](Message<RFCCodes> m) {
			this->onMessage(std::move(m));
		});

		QObject::connect(this->_ui.button_connect, &QPushButton::clicked, this, &HomePage::doConnect);
		QObject::connect(this->_ui.button_login, &QPushButton::clicked, this, &HomePage::doLogin);

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
		//m.header.codeId = Babel::RFCCodes::Debug;
		//m << "i'm from qT5";
		m << static_cast<uint8_t>(username.size()) << username;
		if (!username.empty()) {
			this->connection.send(m);
		}
	}

	void HomePage::onMessage(Message<RFCCodes> m)
	{
		uint16_t codeId;
		uint8_t descLength;

		m >> codeId >> descLength;

		std::string desc;
		Message<RFCCodes>::GetBytes(m, desc, descLength);

		std::cout << "CodeId: " << codeId << " desc: " << desc << std::endl;
	}
}