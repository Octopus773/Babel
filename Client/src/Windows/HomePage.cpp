//
// Created by cbihan on 14/10/2021.
//

#include "HomePage.hpp"
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


		QObject::connect(this->_ui.button_connect, &QPushButton::clicked, this, &HomePage::connect);

		this->_window->show();
		//this->_windowLogin->show();
	}

	void HomePage::connect()
	{
		this->connection.connect(this->_ui.input_address->text().toStdString(), this->_ui.input_port->value());
	}
}