
#ifndef LOGINDLPKFV_H
#define LOGINDLPKFV_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
	QWidget *centralwidget;
	QWidget *verticalLayoutWidget;
	QVBoxLayout *verticalLayout;
	QLabel *label_2;
	QFormLayout *formLayout;
	QLabel *label;
	QLineEdit *input_username;
	QPushButton *button_login;
	QMenuBar *menubar;
	QStatusBar *statusbar;

	void setupUi(QMainWindow *Login)
	{
		if (Login->objectName().isEmpty())
			Login->setObjectName(QString::fromUtf8("Login"));
		Login->resize(495, 279);
		centralwidget = new QWidget(Login);
		centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
		verticalLayoutWidget = new QWidget(centralwidget);
		verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
		verticalLayoutWidget->setGeometry(QRect(10, 10, 451, 201));
		verticalLayout = new QVBoxLayout(verticalLayoutWidget);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		label_2 = new QLabel(verticalLayoutWidget);
		label_2->setObjectName(QString::fromUtf8("label_2"));

		verticalLayout->addWidget(label_2);

		formLayout = new QFormLayout();
		formLayout->setObjectName(QString::fromUtf8("formLayout"));
		label = new QLabel(verticalLayoutWidget);
		label->setObjectName(QString::fromUtf8("label"));

		formLayout->setWidget(1, QFormLayout::LabelRole, label);

		input_username = new QLineEdit(verticalLayoutWidget);
		input_username->setObjectName(QString::fromUtf8("input_username"));
		input_username->setMaxLength(10);

		formLayout->setWidget(1, QFormLayout::FieldRole, input_username);

		button_login = new QPushButton(verticalLayoutWidget);
		button_login->setObjectName(QString::fromUtf8("button_login"));

		formLayout->setWidget(2, QFormLayout::FieldRole, button_login);


		verticalLayout->addLayout(formLayout);

		Login->setCentralWidget(centralwidget);
		menubar = new QMenuBar(Login);
		menubar->setObjectName(QString::fromUtf8("menubar"));
		menubar->setGeometry(QRect(0, 0, 495, 24));
		Login->setMenuBar(menubar);
		statusbar = new QStatusBar(Login);
		statusbar->setObjectName(QString::fromUtf8("statusbar"));
		Login->setStatusBar(statusbar);

		retranslateUi(Login);

		QMetaObject::connectSlotsByName(Login);
	} // setupUi

	void retranslateUi(QMainWindow *Login)
	{
		Login->setWindowTitle(QCoreApplication::translate("Login", "Login", nullptr));
		label_2->setText(QCoreApplication::translate("Login", "Please enter your username for this server", nullptr));
		label->setText(QCoreApplication::translate("Login", "Username", nullptr));
		button_login->setText(QCoreApplication::translate("Login", "Login", nullptr));
	} // retranslateUi

};

namespace Ui {
	class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LOGINDLPKFV_H