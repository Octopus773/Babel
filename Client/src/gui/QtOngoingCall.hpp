#pragma once

#ifndef BABEL_CALL_UIEOTEKK_H
#define BABEL_CALL_UIEOTEKK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_OngoingCall {
public:
    QPushButton *hangUpButton;
    QLabel *label;

    void setupUi(QDialog *OngoingCall);

    void retranslateUi(QDialog *OngoingCall) const;

};

namespace Babel::Ui {
    class OngoingCall : public QDialog, public Ui_OngoingCall {
    Q_OBJECT
    public:
        OngoingCall(QDialog *);

        ~OngoingCall();

        OngoingCall(const OngoingCall &) = delete;

        OngoingCall &operator=(const OngoingCall &) = delete;

    public slots:

        void hangUpButtonCallback();
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // BABEL_CALL_UIEOTEKK_H