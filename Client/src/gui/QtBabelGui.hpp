#pragma once

#include "QtHomePage.hpp"
#include "QtLogin.hpp"
#include "QtOngoingCall.hpp"
#include "QtIncomingCall.hpp"
#include <string>

namespace Babel {
	class QtBabelGui {
	public:
		QtBabelGui();
		QtBabelGui(const QtBabelGui& other) = delete;
		~QtBabelGui() = default;
		void operator=(const QtBabelGui& other) = delete;

	private:
		Ui::HomePage _homePage;
		Ui::LoginPage _loginPage;
		Ui::OngoingCall _ongoingCallPage;
		Ui::IncomingCall _incomingCallPage;
		
		std::string _username;
	};
}