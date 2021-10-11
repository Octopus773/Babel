#pragma once

#ifndef BABEL_CONNECT_UIZAPZDT_H
#define BABEL_CONNECT_UIZAPZDT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton* loginButton;
    QPushButton* cancelButton;
    QPlainTextEdit* userList;

    void setupUi(QDialog* Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(341, 144);
        loginButton = new QPushButton(Dialog);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        loginButton->setGeometry(QRect(30, 70, 111, 31));
        cancelButton = new QPushButton(Dialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(160, 70, 111, 31));
        userList = new QPlainTextEdit(Dialog);
        userList->setObjectName(QStringLiteral("userList"));
        userList->setGeometry(QRect(30, 10, 241, 41));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog* Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        loginButton->setText(QApplication::translate("Dialog", "Login", nullptr));
        cancelButton->setText(QApplication::translate("Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Babel::Ui {
    class LoginPage : public Ui_Dialog {
    public:
        LoginPage(QDialog* Dialog);
        ~LoginPage();
        LoginPage(const LoginPage&) = delete;
        LoginPage &operator=(const LoginPage&) = delete;
    
    public slots:
        void loginButtonCallback();
        void cancelButtonCallback();
        void userListCallback();
    };

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
} // namespace Ui

QT_END_NAMESPACE

#endif // BABEL_CONNECT_UIZAPZDT_H
