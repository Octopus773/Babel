//
// Created by cbihan on 20/09/2021.
//

#pragma once

#include <stdexcept>

namespace Babel::Exception
{
	//! @brief Base Project Exception
	class BabelException : public std::runtime_error
	{
	public:
		//! @brief Base Project Exception.
		//! @note Every project Exceptions should inherit from this class
		explicit BabelException(const std::string &str)
			: std::runtime_error(str)
		{
		}

		//! @brief Default copy constructor
		BabelException(const BabelException &) = default;

		//! @brief Default destructor
		~BabelException() override = default;
	};
}