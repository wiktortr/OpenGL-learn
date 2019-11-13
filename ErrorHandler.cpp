#include "ErrorHandler.h"
#include <exception>

namespace errorHandler {

	void exceptionsHandler() {
		try {
			throw;
		}
		catch (const std::runtime_error& err) {
			std::cout << "runtime error: " << err.what() << "\n";
		}
	}

	void throwRuntimeError(const std::string& note) {
		try {
			throw std::runtime_error(note);
		}
		catch (...) {
			exceptionsHandler(); 
		}
	}
}