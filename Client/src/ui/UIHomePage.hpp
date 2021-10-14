/********************************************************************************
** Form generated from reading UI file 'babelHomePageNTymLt.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BABELHOMEPAGENTYMLT_H
#define BABELHOMEPAGENTYMLT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_babelHome
{
public:
	QWidget *centralwidget;
	QWidget *verticalLayoutWidget;
	QVBoxLayout *verticalLayout;
	QPushButton *button_connect;
	QPushButton *button_login;
	QWidget *formLayoutWidget;
	QFormLayout *formLayout;
	QLabel *label;
	QLineEdit *input_address;
	QLabel *label_2;
	QSpinBox *input_port;
	QMenuBar *menubar;
	QStatusBar *statusbar;

	void setupUi(QMainWindow *babelHome)
	{
		if (babelHome->objectName().isEmpty())
			babelHome->setObjectName(QString::fromUtf8("babelHome"));
		babelHome->resize(800, 600);
		centralwidget = new QWidget(babelHome);
		centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
		verticalLayoutWidget = new QWidget(centralwidget);
		verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
		verticalLayoutWidget->setGeometry(QRect(530, 80, 160, 80));
		verticalLayout = new QVBoxLayout(verticalLayoutWidget);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		button_connect = new QPushButton(verticalLayoutWidget);
		button_connect->setObjectName(QString::fromUtf8("button_connect"));

		verticalLayout->addWidget(button_connect);

		button_login = new QPushButton(verticalLayoutWidget);
		button_login->setObjectName(QString::fromUtf8("button_login"));

		verticalLayout->addWidget(button_login);

		formLayoutWidget = new QWidget(centralwidget);
		formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
		formLayoutWidget->setGeometry(QRect(79, 60, 311, 171));
		formLayout = new QFormLayout(formLayoutWidget);
		formLayout->setObjectName(QString::fromUtf8("formLayout"));
		formLayout->setContentsMargins(0, 0, 0, 0);
		label = new QLabel(formLayoutWidget);
		label->setObjectName(QString::fromUtf8("label"));

		formLayout->setWidget(0, QFormLayout::LabelRole, label);

		input_address = new QLineEdit(formLayoutWidget);
		input_address->setObjectName(QString::fromUtf8("input_address"));
		input_address->setReadOnly(false);

		formLayout->setWidget(0, QFormLayout::FieldRole, input_address);

		label_2 = new QLabel(formLayoutWidget);
		label_2->setObjectName(QString::fromUtf8("label_2"));

		formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

		input_port = new QSpinBox(formLayoutWidget);
		input_port->setObjectName(QString::fromUtf8("input_port"));
		input_port->setMaximum(65535);

		formLayout->setWidget(1, QFormLayout::FieldRole, input_port);

		babelHome->setCentralWidget(centralwidget);
		menubar = new QMenuBar(babelHome);
		menubar->setObjectName(QString::fromUtf8("menubar"));
		menubar->setGeometry(QRect(0, 0, 800, 24));
		babelHome->setMenuBar(menubar);
		statusbar = new QStatusBar(babelHome);
		statusbar->setObjectName(QString::fromUtf8("statusbar"));
		babelHome->setStatusBar(statusbar);

		retranslateUi(babelHome);

		QMetaObject::connectSlotsByName(babelHome);
	} // setupUi

	void retranslateUi(QMainWindow *babelHome)
	{
		babelHome->setWindowTitle(QCoreApplication::translate("babelHome", "Babel", nullptr));
		button_connect->setText(QCoreApplication::translate("babelHome", "connect", nullptr));
		button_login->setText(QCoreApplication::translate("babelHome", "login", nullptr));
		label->setText(QCoreApplication::translate("babelHome", "Address", nullptr));
		label_2->setText(QCoreApplication::translate("babelHome", "Port", nullptr));
	} // retranslateUi

};

namespace Ui {
	class babelHome: public Ui_babelHome {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BABELHOMEPAGENTYMLT_H
