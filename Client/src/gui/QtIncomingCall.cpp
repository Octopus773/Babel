#include "QtIncomingCall.hpp"

void Ui_IncomingCall::setupUi(QDialog *IncomingCall)
{
    if (IncomingCall->objectName().isEmpty())
        IncomingCall->setObjectName(QStringLiteral("IncomingCall"));
    IncomingCall->resize(400, 134);
    denyCallButton = new QPushButton(IncomingCall);
    denyCallButton->setObjectName(QStringLiteral("denyCallButton"));
    denyCallButton->setGeometry(QRect(230, 70, 71, 51));
    acceptCallButton = new QPushButton(IncomingCall);
    acceptCallButton->setObjectName(QStringLiteral("acceptCallButton"));
    acceptCallButton->setGeometry(QRect(80, 70, 71, 51));
    callFromUserLabel = new QLabel(IncomingCall);
    callFromUserLabel->setObjectName(QStringLiteral("callFromUserLabel"));
    callFromUserLabel->setGeometry(QRect(130, 30, 101, 16));

    retranslateUi(IncomingCall);

    QMetaObject::connectSlotsByName(IncomingCall);
} // setupUi

void Ui_IncomingCall::retranslateUi(QDialog* IncomingCall)
{
    IncomingCall->setWindowTitle(QApplication::translate("IncomingCall", "Dialog", nullptr));
    denyCallButton->setText(QApplication::translate("IncomingCall", "Deny Call", nullptr));
    acceptCallButton->setText(QApplication::translate("IncomingCall", "Accept Call", nullptr));
    callFromUserLabel->setText(QApplication::translate("IncomingCall", "<user> is calling you", nullptr));
}

using namespace Babel::Ui

IncomingCall::~IncomingCall()
{
    delete this->denyCallButton;
    delete this->acceptCallButton;
    delete this->callFromUserLabel;
}


IncomingCall::IncomingCall(QDialog* Dialog)
{
    this->setupUi(Dialog);
    connect(this->acceptCallButton, SIGNAL(clicked()), this, SLOT(disconnectButtonCallback()));
    connect(this->denyCallButton, SIGNAL(clicked()), this, SLOT(callUserButtonCallback()));
}

void IncomingCall::denyCallButtonCallback()
{

}

void IncomingCall::acceptCallButtonCallback()
{

}