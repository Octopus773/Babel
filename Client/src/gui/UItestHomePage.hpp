//
// Created by cbihan on 14/10/2021.
//

#ifndef BABELHOMEPAGEYYVLFJ_H
#define BABELHOMEPAGEYYVLFJ_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
	QMenuBar *menubar;
	QStatusBar *statusbar;

	void setupUi(QMainWindow *babelHome);

	void retranslateUi(QMainWindow *babelHome)
	{
		babelHome->setWindowTitle(QCoreApplication::translate("babelHome", "Babel", nullptr));
		button_connect->setText(QCoreApplication::translate("babelHome", "connect", nullptr));
		button_login->setText(QCoreApplication::translate("babelHome", "login", nullptr));
	} // retranslateUi

};

namespace Ui {
	class babelHome: public Ui_babelHome {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BABELHOMEPAGEYYVLFJ_H

