#include "QtIncomingCall.hpp"

QT_BEGIN_NAMESPACE

void Ui_IncomingCall::setupUi(QDialog *IncomingCall) {
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

void Ui_IncomingCall::retranslateUi(QDialog *IncomingCall) {
    IncomingCall->setWindowTitle(QApplication::translate("IncomingCall", "Dialog", nullptr));
    denyCallButton->setText(QApplication::translate("IncomingCall", "Deny Call", nullptr));
    acceptCallButton->setText(QApplication::translate("IncomingCall", "Accept Call", nullptr));
    callFromUserLabel->setText(QApplication::translate("IncomingCall", "<user> is calling you", nullptr));
}

Babel::Ui::IncomingCall::~IncomingCall() {
    delete this->denyCallButton;
    delete this->acceptCallButton;
    delete this->callFromUserLabel;
}


Babel::Ui::IncomingCall::IncomingCall(QDialog *Dialog) : QDialog(Dialog), Ui_IncomingCall() {
    this->setupUi(Dialog);
    connect(this->acceptCallButton, SIGNAL(clicked()), this, SLOT(acceptCallButtonCallback()));
    connect(this->denyCallButton, SIGNAL(clicked()), this, SLOT(denyCallButtonCallback()));
}

void Babel::Ui::IncomingCall::denyCallButtonCallback() {

}

void Babel::Ui::IncomingCall::acceptCallButtonCallback() {

}

QT_END_NAMESPACE
