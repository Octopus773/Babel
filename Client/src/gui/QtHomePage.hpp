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

    void setupUi(QDialog* HomePage)
    {
        if (HomePage->objectName().isEmpty())
            HomePage->setObjectName(QStringLiteral("HomePage"));
        HomePage->resize(466, 290);
        disconnectButton = new QPushButton(HomePage);
        disconnectButton->setObjectName(QStringLiteral("disconnectButton"));
        disconnectButton->setGeometry(QRect(360, 240, 75, 23));
        callUserButton = new QPushButton(HomePage);
        callUserButton->setObjectName(QStringLiteral("callUserButton"));
        callUserButton->setGeometry(QRect(360, 200, 75, 23));
        onlineUsersTextBox = new QPlainTextEdit(HomePage);
        onlineUsersTextBox->setObjectName(QStringLiteral("onlineUsersTextBox"));
        onlineUsersTextBox->setGeometry(QRect(60, 60, 281, 201));
        onlineUsersLabel = new QLabel(HomePage);
        onlineUsersLabel->setObjectName(QStringLiteral("onlineUsersLabel"));
        onlineUsersLabel->setGeometry(QRect(40, 30, 111, 16));

        retranslateUi(HomePage);

        QMetaObject::connectSlotsByName(HomePage);
    } // setupUi

    void retranslateUi(QDialog* HomePage)
    {
        HomePage->setWindowTitle(QApplication::translate("HomePage", "Dialog", nullptr));
        disconnectButton->setText(QApplication::translate("HomePage", "Disconnect", nullptr));
        callUserButton->setText(QApplication::translate("HomePage", "Call", nullptr));
        onlineUsersLabel->setText(QApplication::translate("HomePage", "Other users on server", nullptr));
    } // retranslateUi

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

    HomePage::HomePage(QDialog* Dialog)
    {
        this->setupUi(Dialog);
        QDialog::connect(this->disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectButtonCallback()));
        QDialog::connect(this->callUserButton, SIGNAL(clicked()), this, SLOT(callUserButtonCallback()));
    }

    HomePage::~HomePage()
    {
        delete this->disconnectButton;
        delete this->callUserButton;
        delete this->onlineUsersTextBox;
        delete this->onlineUsersLabel;
    }

    void HomePage::disconnectButtonCallback()
    {

    }

    void HomePage::callUserButtonCallback()
    {

    }
} // namespace Ui

QT_END_NAMESPACE

#endif // USERS_ON_SERVER_UIJVRRFH_H