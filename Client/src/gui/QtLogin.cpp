#include "QtLogin.hpp"

void Ui_Dialog::setupUi(QDialog* Dialog)
{
    if (Dialog->objectName().isEmpty())
        Dialog->setObjectName(QStringLiteral("Dialog"));
    Dialog->resize(341, 144);
    pushButton = new QPushButton(Dialog);
    pushButton->setObjectName(QStringLiteral("pushButton"));
    pushButton->setGeometry(QRect(30, 70, 111, 31));
    pushButton_2 = new QPushButton(Dialog);
    pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
    pushButton_2->setGeometry(QRect(160, 70, 111, 31));
    plainTextEdit = new QPlainTextEdit(Dialog);
    plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
    plainTextEdit->setGeometry(QRect(30, 10, 241, 41));

    retranslateUi(Dialog);

    QMetaObject::connectSlotsByName(Dialog);
}

void Ui_Dialog::retranslateUi(QDialog* Dialog)
{
    Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
    loginButton->setText(QApplication::translate("Dialog", "Login", nullptr));
    cancelButton->setText(QApplication::translate("Dialog", "Cancel", nullptr));
} // retranslateUi


using namespace Babel::Ui;

LoginPage::~LoginPage()
{
    delete this->userList;
    delete this->loginButton;
    delete this->cancelButton;
}

LoginPage::LoginPage(QDialog* Dialog)
{
    this->setupUi(Dialog);
    connect(this->loginButton, SIGNAL(clicked()), this, SLOT(loginButtonCallback()));
    connect(this->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonCallback()));
    connect(this->userList, SIGNAL(clicked()), this, SLOT(userListCallback()));
}

void LoginPage::loginButtonCallback()
{

}

void LoginPage::cancelButtonCallback()
{

}

void LoginPage::userListCallback()
{

}