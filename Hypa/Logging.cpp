/*
***********************************************************************
*                                                                     *
* Drizzle3D � 2024 by Ronit D'silva is licensed under CC BY-NC-SA 4.0 *
*                                                                     *
***********************************************************************
*/
#include "Logging.h"

namespace Hypa {
	Logging::Logging() {

	}

	void Logging::Error(std::string message, std::string who) {
		std::cout << who << "Error: " << message << "\n";
	}

	void Logging::Warning(std::string message, std::string who) {
		std::cout << who << "Warning: " << message << "\n";
	}

	void Logging::Info(std::string message, std::string who) {
#ifdef _DEBUG
		std::cout << who << "INFO: " << message << "\n";
#elif NDEBUG
		std::cout << who << "INFO: " << message << "\n";
#endif
	}
}