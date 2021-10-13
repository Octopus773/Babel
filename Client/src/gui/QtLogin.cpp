#include "QtLogin.hpp"

QT_BEGIN_NAMESPACE

void Ui_ConnectWindow::setupUi(QDialog* ConnectWindow)
{
    if (ConnectWindow->objectName().isEmpty())
        ConnectWindow->setObjectName(QStringLiteral("ConnectWindow"));
    ConnectWindow->resize(341, 144);
    loginButton = new QPushButton(ConnectWindow);
    loginButton->setObjectName(QStringLiteral("loginButton"));
    loginButton->setGeometry(QRect(30, 70, 111, 31));
    cancelButton = new QPushButton(ConnectWindow);
    cancelButton->setObjectName(QStringLiteral("cancelButton"));
    cancelButton->setGeometry(QRect(200, 70, 111, 31));
    usernameBox = new QLineEdit(ConnectWindow);
    usernameBox->setObjectName(QStringLiteral("usernameBox"));
    usernameBox->setGeometry(QRect(30, 20, 281, 31));

    retranslateUi(ConnectWindow);

    QMetaObject::connectSlotsByName(ConnectWindow);
}

void Ui_ConnectWindow::retranslateUi(QDialog* ConnectWindow)
{
    ConnectWindow->setWindowTitle(QApplication::translate("ConnectWindow", "Dialog", nullptr));
    loginButton->setText(QApplication::translate("ConnectWindow", "Login", nullptr));
    cancelButton->setText(QApplication::translate("ConnectWindow", "Cancel", nullptr));
    usernameBox->setText(QApplication::translate("ConnectWindow", "username", nullptr));
} // retranslateUi

Babel::Ui::LoginPage::~LoginPage()
{
    delete this->usernameBox;
    delete this->loginButton;
    delete this->cancelButton;
}

Babel::Ui::LoginPage::LoginPage(QDialog* Dialog): QObject(), Ui_ConnectWindow()
{
    this->setupUi(Dialog);
    connect(this->loginButton, SIGNAL(clicked()), this, SLOT(loginButtonCallback()));
    connect(this->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonCallback()));
    //connect(this->usernameBox, SIGNAL(clicked()), this, SLOT(userListCallback()));
}

void Babel::Ui::LoginPage::loginButtonCallback()
{

}

void Babel::Ui::LoginPage::cancelButtonCallback()
{

}

void Babel::Ui::LoginPage::userListCallback()
{

}

QT_END_NAMESPACE
