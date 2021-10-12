#pragma once

#ifndef INCOMING_CALL_UILKTZJO_H
#define INCOMING_CALL_UILKTZJO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_IncomingCall
{
public:
    QPushButton* denyCallButton;
    QPushButton* acceptCallButton;
    QLabel* callFromUserLabel;

    void setupUi(QDialog* IncomingCall)
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

    void retranslateUi(QDialog* IncomingCall)
    {
        IncomingCall->setWindowTitle(QApplication::translate("IncomingCall", "Dialog", nullptr));
        denyCallButton->setText(QApplication::translate("IncomingCall", "Deny Call", nullptr));
        acceptCallButton->setText(QApplication::translate("IncomingCall", "Accept Call", nullptr));
        callFromUserLabel->setText(QApplication::translate("IncomingCall", "<user> is calling you", nullptr));
    } // retranslateUi

};

namespace Babel::Ui {
    class IncomingCall : public Ui_IncomingCall {
        IncomingCall(QDialog*);
        ~IncomingCall();
        IncomingCall(const IncomingCall&) = delete;
        IncomingCall& operator=(const IncomingCall&) = delete;

    public slots:
        void denyCallButtonCallback();
        void acceptCallButtonCallback();
    };

    IncomingCall::IncomingCall(QDialog* Dialog)
    {
        this->setupUi(Dialog);
        connect(this->acceptCallButton, SIGNAL(clicked()), this, SLOT(disconnectButtonCallback()));
        connect(this->denyCallButton, SIGNAL(clicked()), this, SLOT(callUserButtonCallback()));
    }

    IncomingCall::~IncomingCall()
    {
        delete this->denyCallButton;
        delete this->acceptCallButton;
        delete this->callFromUserLabel;
    }

    void IncomingCall::denyCallButtonCallback()
    {

    }

    void IncomingCall::acceptCallButtonCallback()
    {

    }
};
} // namespace Ui

QT_END_NAMESPACE

#endif // INCOMING_CALL_UILKTZJO_H
