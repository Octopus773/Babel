//
// Created by cbihan on 14/10/2021.
//

#pragma once

#include "ui/UIHomePage.hpp"
#include "ui/UiLogin.hpp"
#include "Network/QtTCPConnection.hpp"
#include <QMainWindow>
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


		void onMessage(Message<RFCCodes> m);

		void doConnect();

		void doLogin();
	};
}


