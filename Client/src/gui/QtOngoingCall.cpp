#include "QtOngoingCall.hpp"

QT_BEGIN_NAMESPACE

void Ui_OngoingCall::setupUi(QDialog *OngoingCall) {
    if (OngoingCall->objectName().isEmpty())
        OngoingCall->setObjectName(QStringLiteral("OngoingCall"));
    OngoingCall->resize(342, 126);
    hangUpButton = new QPushButton(OngoingCall);
    hangUpButton->setObjectName(QStringLiteral("hangUpButton"));
    hangUpButton->setGeometry(QRect(130, 60, 75, 51));
    label = new QLabel(OngoingCall);
    label->setObjectName(QStringLiteral("label"));
    label->setGeometry(QRect(100, 30, 141, 16));

    retranslateUi(OngoingCall);

    QMetaObject::connectSlotsByName(OngoingCall);
} // setupUi

void Ui_OngoingCall::retranslateUi(QDialog *OngoingCall) const {
    OngoingCall->setWindowTitle(QApplication::translate("OngoingCall", "Dialog", nullptr));
    hangUpButton->setText(QApplication::translate("OngoingCall", "Hang Up", nullptr));
    label->setText(QApplication::translate("OngoingCall", "You are in a call with <user>", nullptr));
}

using namespace Babel::Ui;

OngoingCall::~OngoingCall() {
    delete this->label;
    delete this->hangUpButton;
}

OngoingCall::OngoingCall(QDialog *Dialog) : QDialog(), Ui_OngoingCall() {
    this->setupUi(Dialog);
    connect(this->hangUpButton, SIGNAL(clicked()), this, SLOT(hangUpButtonCallback()));
}

void OngoingCall::hangUpButtonCallback() {

}

QT_END_NAMESPACE
