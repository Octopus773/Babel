#pragma once

#include "QtHomePage.hpp"
#include "QtLogin.hpp"
#include "QtOngoingCall.hpp"
#include "QtIncomingCall.hpp"
#include <string>

namespace Babel {
	class QtBabelGui {
	public:
		QtBabelGui() : _homePage(Ui::HomePage(new QDialog)), _loginPage(Ui::LoginPage(new QDialog)), _ongoingCallPage(Ui::OngoingCallPage(new QDialog)), _incomingCallPage(new QDialog));
		QtBabelGui(const QtBabelGui& other) = default;
		~QtBabelGui() = default;
		void& operator=(const QtBabelGui& other) = delete;

	private:
		Ui::HomePage _homePage;
		Ui::LoginPage _loginPage;
		Ui::OngoingCall _ongoingCallPage;
		Ui::IncomingCall _incomingCallPage;
		
		std::string _username;
	};

	QtBabelGui::QtBabelGui()
	{
		_loginPage.show();
	}
}