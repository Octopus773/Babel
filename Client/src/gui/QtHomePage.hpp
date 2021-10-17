#pragma once

#ifndef USERS_ON_SERVER_UIJVRRFH_H
#define USERS_ON_SERVER_UIJVRRFH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QMainWindow>

QT_BEGIN_NAMESPACE

class Ui_HomePage {
public:
    QPushButton *disconnectButton;
    QPushButton *callUserButton;
    QPlainTextEdit *onlineUsersTextBox;
    QLabel *onlineUsersLabel;

    void setupUi(QWidget *HomePage);

    // retranslateUi

};

namespace Babel::Ui {
    class HomePage : public QObject {
    Q_OBJECT
    public:
        explicit HomePage(QWidget *parent = nullptr);

        ~HomePage() override;

        HomePage(const HomePage &) = delete;

        HomePage &operator=(const HomePage &) = delete;


        QMainWindow *window;

        Ui_HomePage _ui;
    public slots:

        void disconnectButtonCallback();

        void callUserButtonCallback();
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // USERS_ON_SERVER_UIJVRRFH_H