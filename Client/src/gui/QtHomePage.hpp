#pragma once

#ifndef USERS_ON_SERVER_UIJVRRFH_H
#define USERS_ON_SERVER_UIJVRRFH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_HomePage
{
public:
    QPushButton* disconnectButton;
    QPushButton* callUserButton;
    QPlainTextEdit* onlineUsersTextBox;
    QLabel* onlineUsersLabel;

    void setupUi(QDialog* HomePage);

    void retranslateUi(QDialog* HomePage); // retranslateUi

};

namespace Babel::Ui {
    class HomePage : public QObject, public Ui_HomePage
    {
	Q_OBJECT
    public:
        HomePage(QDialog*);
        ~HomePage();
        HomePage(const HomePage&) = delete;
        HomePage &operator=(const HomePage&) = delete;

    public slots:
        void disconnectButtonCallback();
        void callUserButtonCallback();
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // USERS_ON_SERVER_UIJVRRFH_H