#pragma once

#ifndef BABEL_CONNECT_UIZAPZDT_H
#define BABEL_CONNECT_UIZAPZDT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConnectWindow
{
public:
    QPushButton* loginButton;
    QPushButton* cancelButton;
    QLineEdit* usernameBox;

    void setupUi(QDialog* Dialog);
    void retranslateUi(QDialog* Dialog);
};

namespace Babel::Ui {
    class LoginPage : public QDialog, public Ui_ConnectWindow {
        Q_OBJECT
	public:
        explicit LoginPage(QDialog* Dialog = nullptr);
        ~LoginPage() override;
        LoginPage(const LoginPage&) = delete;
        LoginPage &operator=(const LoginPage&) = delete;
    
    public slots:
        void loginButtonCallback();
        void cancelButtonCallback();
        void userListCallback();
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // BABEL_CONNECT_UIZAPZDT_H
