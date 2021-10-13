#include "QtHomePage.hpp"

QT_BEGIN_NAMESPACE

void Ui_HomePage::setupUi(QDialog *HomePage)
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

void Ui_HomePage::retranslateUi(QDialog * HomePage)
{
    HomePage->setWindowTitle(QApplication::translate("HomePage", "Dialog", nullptr));
    disconnectButton->setText(QApplication::translate("HomePage", "Disconnect", nullptr));
    callUserButton->setText(QApplication::translate("HomePage", "Call", nullptr));
    onlineUsersLabel->setText(QApplication::translate("HomePage", "Other users on server", nullptr));
}

Babel::Ui::HomePage::HomePage(QDialog* Dialog): QObject(), Ui_HomePage()
{
    this->setupUi(Dialog);
    connect(this->disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectButtonCallback()));
    connect(this->callUserButton, SIGNAL(clicked()), this, SLOT(callUserButtonCallback()));
}

Babel::Ui::HomePage::~HomePage()
{
    delete this->disconnectButton;
    delete this->callUserButton;
    delete this->onlineUsersTextBox;
    delete this->onlineUsersLabel;
}

void Babel::Ui::HomePage::disconnectButtonCallback()
{

}

void Babel::Ui::HomePage::callUserButtonCallback()
{

}

QT_END_NAMESPACE
