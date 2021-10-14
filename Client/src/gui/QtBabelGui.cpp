
#include "QtBabelGui.hpp"

Babel::QtBabelGui::QtBabelGui()
	: _homePage(Ui::HomePage(new QDialog)),
	_loginPage(Ui::LoginPage(new QDialog)),
	_ongoingCallPage(Ui::OngoingCall(new QDialog)),
	_incomingCallPage(new QDialog)
{
	_loginPage.show();
}