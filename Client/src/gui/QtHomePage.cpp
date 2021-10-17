#include "QtHomePage.hpp"

QT_BEGIN_NAMESPACE

void Ui_HomePage::setupUi(QWidget *HomePage) {
    if (HomePage->objectName().isEmpty())
        HomePage->setObjectName(QStringLiteral("HomePage"));
    HomePage->resize(466, 290);
    disconnectButton = new QPushButton(HomePage);
    disconnectButton->setObjectName("disconnectButton");
    disconnectButton->setGeometry(QRect(360, 240, 75, 23));
    callUserButton = new QPushButton(HomePage);
    callUserButton->setObjectName(QString::fromUtf8("callUserButton"));
    callUserButton->setGeometry(QRect(360, 200, 75, 23));
    onlineUsersTextBox = new QPlainTextEdit(HomePage);
    onlineUsersTextBox->setObjectName(QStringLiteral("onlineUsersTextBox"));
    onlineUsersTextBox->setGeometry(QRect(60, 60, 281, 201));
    onlineUsersLabel = new QLabel(HomePage);
    onlineUsersLabel->setObjectName(QStringLiteral("onlineUsersLabel"));
    onlineUsersLabel->setGeometry(QRect(40, 30, 111, 16));

    // retranslateUi(HomePage);

    QMetaObject::connectSlotsByName(HomePage);
} // setupUi

Babel::Ui::HomePage::HomePage(QWidget *)
        : window(new QMainWindow),
          _ui({}) {
    this->_ui.setupUi(this->window);
    QMainWindow::connect(this->_ui.disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectButtonCallback()));
    QMainWindow::connect(this->_ui.callUserButton, SIGNAL(clicked()), this, SLOT(callUserButtonCallback()));
    this->window->show();
}

Babel::Ui::HomePage::~HomePage() {
    delete this->window;
}

void Babel::Ui::HomePage::disconnectButtonCallback() {

}

void Babel::Ui::HomePage::callUserButtonCallback() {

}

QT_END_NAMESPACE
