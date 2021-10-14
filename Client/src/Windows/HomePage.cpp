//
// Created by cbihan on 14/10/2021.
//

#include "HomePage.hpp"
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"
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


		QObject::connect(this->_ui.button_connect, &QPushButton::clicked, this, &HomePage::doConnect);

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
		this->connection.send(m);
	}
}