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

    void setupUi(QDialog* Dialog);
    void retranslateUi(QDialog* Dialog);
};

namespace Babel::Ui {
    class LoginPage : public QObject, public Ui_Dialog {
        Q_OBJECT
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
} // namespace Ui

QT_END_NAMESPACE

#endif // BABEL_CONNECT_UIZAPZDT_H
