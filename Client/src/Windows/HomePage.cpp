//
// Created by cbihan on 14/10/2021.
//

#include "HomePage.hpp"
#include "UItestHomePage.hpp"
#include <QMainWindow>

void Ui_babelHome::setupUi(QMainWindow *babelHome)
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

namespace Babel
{

	HomePage::HomePage()
		: _window(new QMainWindow()),
		  _ui()
	{

		this->_ui.setupUi(this->_window);
		this->_window->show();
	}
}