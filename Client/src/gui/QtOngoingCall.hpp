#pragma once

#ifndef BABEL_CALL_UIEOTEKK_H
#define BABEL_CALL_UIEOTEKK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_OngoingCall
{
public:
    QPushButton* hangUpButton;
    QLabel* label;

    void setupUi(QDialog* OngoingCall)
    {
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

    void retranslateUi(QDialog* OngoingCall)
    {
        OngoingCall->setWindowTitle(QApplication::translate("OngoingCall", "Dialog", nullptr));
        hangUpButton->setText(QApplication::translate("OngoingCall", "Hang Up", nullptr));
        label->setText(QApplication::translate("OngoingCall", "You are in a call with <user>", nullptr));
    } // retranslateUi

};

namespace Babel::Ui {
    class OngoingCall : public QDialog, public Ui_OngoingCall
    {
		Q_OBJECT
    public:
        OngoingCall(QDialog*);
        ~OngoingCall();
        OngoingCall(const OngoingCall&) = delete;
        OngoingCall& operator=(const OngoingCall&) = delete;

    public slots:
        void hangUpButtonCallback();
    };

    OngoingCall::~OngoingCall()
    {
        delete this->label;
        delete this->hangUpButton;
    }

    OngoingCall::OngoingCall(QDialog* Dialog)
    {
        this->setupUi(Dialog);
        connect(this->hangUpButton, SIGNAL(clicked()), this, SLOT(hangUpButtonCallback()));
    }

    void OngoingCall::hangUpButtonCallback()
    {

    }
} // namespace Ui

QT_END_NAMESPACE

#endif // BABEL_CALL_UIEOTEKK_H