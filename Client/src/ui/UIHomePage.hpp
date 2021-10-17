/********************************************************************************
** Form generated from reading UI file 'babelHomePageuxSMrs.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BABELHOMEPAGEUXSMRS_H
#define BABELHOMEPAGEUXSMRS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_babelHome
{
public:
	QWidget *centralwidget;
	QVBoxLayout *verticalLayout;
	QTabWidget *tab_handler;
	QWidget *page_connection;
	QWidget *formLayoutWidget_2;
	QGridLayout *gridLayout_3;
	QLabel *label;
	QLineEdit *input_address;
	QLabel *label_2;
	QSpinBox *input_port;
	QPushButton *button_connect;
	QWidget *page_login;
	QWidget *formLayoutWidget;
	QGridLayout *gridLayout_2;
	QLabel *label_3;
	QLineEdit *input_login_username;
	QPushButton *button_login;
	QWidget *page_server;
	QListWidget *output_connected_user_list;
	QWidget *formLayoutWidget_3;
	QFormLayout *formLayout;
	QLabel *label_4;
	QLabel *output_selected_username;
	QLabel *label_5;
	QCheckBox *output_can_be_called;
	QLabel *label_6;
	QPushButton *button_call_user;
	QPushButton *button_refresh_connected_user_list;
	QWidget *page_call;
	QLabel *label_7;
	QWidget *gridLayoutWidget;
	QGridLayout *gridLayout;
	QListWidget *output_list_call_members;
	QLabel *label_8;
	QPushButton *button_hang_up;

	void setupUi(QMainWindow *babelHome)
	{
		if (babelHome->objectName().isEmpty())
			babelHome->setObjectName(QString::fromUtf8("babelHome"));
		babelHome->resize(800, 600);
		centralwidget = new QWidget(babelHome);
		centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
		verticalLayout = new QVBoxLayout(centralwidget);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		tab_handler = new QTabWidget(centralwidget);
		tab_handler->setObjectName(QString::fromUtf8("tab_handler"));
		tab_handler->setEnabled(true);
		tab_handler->setTabShape(QTabWidget::Rounded);
		page_connection = new QWidget();
		page_connection->setObjectName(QString::fromUtf8("page_connection"));
		formLayoutWidget_2 = new QWidget(page_connection);
		formLayoutWidget_2->setObjectName(QString::fromUtf8("formLayoutWidget_2"));
		formLayoutWidget_2->setGeometry(QRect(90, 20, 501, 96));
		gridLayout_3 = new QGridLayout(formLayoutWidget_2);
		gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
		gridLayout_3->setContentsMargins(0, 0, 0, 0);
		label = new QLabel(formLayoutWidget_2);
		label->setObjectName(QString::fromUtf8("label"));

		gridLayout_3->addWidget(label, 0, 0, 1, 1);

		input_address = new QLineEdit(formLayoutWidget_2);
		input_address->setObjectName(QString::fromUtf8("input_address"));
		input_address->setReadOnly(false);

		gridLayout_3->addWidget(input_address, 0, 1, 1, 1);

		label_2 = new QLabel(formLayoutWidget_2);
		label_2->setObjectName(QString::fromUtf8("label_2"));

		gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

		input_port = new QSpinBox(formLayoutWidget_2);
		input_port->setObjectName(QString::fromUtf8("input_port"));
		input_port->setMaximum(65535);

		gridLayout_3->addWidget(input_port, 1, 1, 1, 1);

		button_connect = new QPushButton(formLayoutWidget_2);
		button_connect->setObjectName(QString::fromUtf8("button_connect"));

		gridLayout_3->addWidget(button_connect, 2, 1, 1, 1);

		tab_handler->addTab(page_connection, QString());
		page_login = new QWidget();
		page_login->setObjectName(QString::fromUtf8("page_login"));
		page_login->setEnabled(true);
		formLayoutWidget = new QWidget(page_login);
		formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
		formLayoutWidget->setGeometry(QRect(50, 30, 651, 62));
		gridLayout_2 = new QGridLayout(formLayoutWidget);
		gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
		gridLayout_2->setContentsMargins(0, 0, 0, 0);
		label_3 = new QLabel(formLayoutWidget);
		label_3->setObjectName(QString::fromUtf8("label_3"));

		gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

		input_login_username = new QLineEdit(formLayoutWidget);
		input_login_username->setObjectName(QString::fromUtf8("input_login_username"));
		input_login_username->setMaxLength(10);

		gridLayout_2->addWidget(input_login_username, 0, 1, 1, 1);

		button_login = new QPushButton(formLayoutWidget);
		button_login->setObjectName(QString::fromUtf8("button_login"));

		gridLayout_2->addWidget(button_login, 1, 1, 1, 1);

		tab_handler->addTab(page_login, QString());
		page_server = new QWidget();
		page_server->setObjectName(QString::fromUtf8("page_server"));
		output_connected_user_list = new QListWidget(page_server);
		output_connected_user_list->setObjectName(QString::fromUtf8("output_connected_user_list"));
		output_connected_user_list->setGeometry(QRect(30, 50, 381, 431));
		formLayoutWidget_3 = new QWidget(page_server);
		formLayoutWidget_3->setObjectName(QString::fromUtf8("formLayoutWidget_3"));
		formLayoutWidget_3->setGeometry(QRect(440, 90, 311, 102));
		formLayout = new QFormLayout(formLayoutWidget_3);
		formLayout->setObjectName(QString::fromUtf8("formLayout"));
		formLayout->setContentsMargins(0, 0, 0, 0);
		label_4 = new QLabel(formLayoutWidget_3);
		label_4->setObjectName(QString::fromUtf8("label_4"));

		formLayout->setWidget(0, QFormLayout::LabelRole, label_4);

		output_selected_username = new QLabel(formLayoutWidget_3);
		output_selected_username->setObjectName(QString::fromUtf8("output_selected_username"));

		formLayout->setWidget(0, QFormLayout::FieldRole, output_selected_username);

		label_5 = new QLabel(formLayoutWidget_3);
		label_5->setObjectName(QString::fromUtf8("label_5"));

		formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

		output_can_be_called = new QCheckBox(formLayoutWidget_3);
		output_can_be_called->setObjectName(QString::fromUtf8("output_can_be_called"));
		output_can_be_called->setEnabled(false);
		QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		sizePolicy.setHeightForWidth(output_can_be_called->sizePolicy().hasHeightForWidth());
		output_can_be_called->setSizePolicy(sizePolicy);
		output_can_be_called->setLayoutDirection(Qt::LeftToRight);
		output_can_be_called->setAutoFillBackground(false);
		output_can_be_called->setCheckable(true);
		output_can_be_called->setChecked(false);
		output_can_be_called->setAutoExclusive(false);
		output_can_be_called->setTristate(false);

		formLayout->setWidget(1, QFormLayout::FieldRole, output_can_be_called);

		label_6 = new QLabel(formLayoutWidget_3);
		label_6->setObjectName(QString::fromUtf8("label_6"));

		formLayout->setWidget(1, QFormLayout::LabelRole, label_6);

		button_call_user = new QPushButton(page_server);
		button_call_user->setObjectName(QString::fromUtf8("button_call_user"));
		button_call_user->setEnabled(false);
		button_call_user->setGeometry(QRect(470, 200, 235, 27));
		button_refresh_connected_user_list = new QPushButton(page_server);
		button_refresh_connected_user_list->setObjectName(QString::fromUtf8("button_refresh_connected_user_list"));
		button_refresh_connected_user_list->setGeometry(QRect(30, 490, 87, 27));
		tab_handler->addTab(page_server, QString());
		page_call = new QWidget();
		page_call->setObjectName(QString::fromUtf8("page_call"));
		label_7 = new QLabel(page_call);
		label_7->setObjectName(QString::fromUtf8("label_7"));
		label_7->setGeometry(QRect(340, 10, 101, 41));
		gridLayoutWidget = new QWidget(page_call);
		gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
		gridLayoutWidget->setGeometry(QRect(160, 60, 461, 321));
		gridLayout = new QGridLayout(gridLayoutWidget);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		gridLayout->setContentsMargins(0, 0, 0, 0);
		output_list_call_members = new QListWidget(gridLayoutWidget);
		output_list_call_members->setObjectName(QString::fromUtf8("output_list_call_members"));

		gridLayout->addWidget(output_list_call_members, 1, 0, 1, 1);

		label_8 = new QLabel(gridLayoutWidget);
		label_8->setObjectName(QString::fromUtf8("label_8"));

		gridLayout->addWidget(label_8, 0, 0, 1, 1);

		button_hang_up = new QPushButton(gridLayoutWidget);
		button_hang_up->setObjectName(QString::fromUtf8("button_hang_up"));

		gridLayout->addWidget(button_hang_up, 2, 0, 1, 1);

		tab_handler->addTab(page_call, QString());

		verticalLayout->addWidget(tab_handler);

		babelHome->setCentralWidget(centralwidget);

		retranslateUi(babelHome);

		tab_handler->setCurrentIndex(0);


		QMetaObject::connectSlotsByName(babelHome);
	} // setupUi

	void retranslateUi(QMainWindow *babelHome)
	{
		babelHome->setWindowTitle(QCoreApplication::translate("babelHome", "Babel", nullptr));
		label->setText(QCoreApplication::translate("babelHome", "Address", nullptr));
		label_2->setText(QCoreApplication::translate("babelHome", "Port", nullptr));
		button_connect->setText(QCoreApplication::translate("babelHome", "connect", nullptr));
		tab_handler->setTabText(tab_handler->indexOf(page_connection), QCoreApplication::translate("babelHome", "Connection", nullptr));
		label_3->setText(QCoreApplication::translate("babelHome", "Username", nullptr));
		button_login->setText(QCoreApplication::translate("babelHome", "login", nullptr));
		tab_handler->setTabText(tab_handler->indexOf(page_login), QCoreApplication::translate("babelHome", "Login", nullptr));
		label_4->setText(QCoreApplication::translate("babelHome", "Username", nullptr));
		output_selected_username->setText(QCoreApplication::translate("babelHome", "selected_username", nullptr));
		label_5->setText(QString());
		output_can_be_called->setText(QString());
		label_6->setText(QCoreApplication::translate("babelHome", "Can be called", nullptr));
		button_call_user->setText(QCoreApplication::translate("babelHome", "Call user", nullptr));
		button_refresh_connected_user_list->setText(QCoreApplication::translate("babelHome", "Refresh", nullptr));
		tab_handler->setTabText(tab_handler->indexOf(page_server), QCoreApplication::translate("babelHome", "Server", nullptr));
		label_7->setText(QCoreApplication::translate("babelHome", "Ongoing call", nullptr));
		label_8->setText(QCoreApplication::translate("babelHome", "Call members", nullptr));
		button_hang_up->setText(QCoreApplication::translate("babelHome", "Hang Up", nullptr));
		tab_handler->setTabText(tab_handler->indexOf(page_call), QCoreApplication::translate("babelHome", "Call", nullptr));
	} // retranslateUi

};

namespace Ui {
	class babelHome: public Ui_babelHome {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BABELHOMEPAGEUXSMRS_H
