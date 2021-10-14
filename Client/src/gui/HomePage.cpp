//
// Created by cbihan on 14/10/2021.
//

#include "HomePage.hpp"
#include <QMainWindow>

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