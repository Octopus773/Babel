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

    void setupUi(QDialog* IncomingCall);
    void retranslateUi(QDialog* IncomingCall);
};

namespace Babel::Ui {
    class IncomingCall : public QObject, public Ui_IncomingCall {
        Q_OBJECT
    public:

	    ~IncomingCall();

	    IncomingCall(QDialog*);

	    IncomingCall(const IncomingCall&) = delete;
        IncomingCall& operator=(const IncomingCall&) = delete;

    public slots:
        void denyCallButtonCallback();
        void acceptCallButtonCallback();
    };

}; // namespace Ui

QT_END_NAMESPACE

#endif // INCOMING_CALL_UILKTZJO_H
