#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

#include <iostream>
#include <string>

namespace errorHandler {
	void exceptionsHandler();
	void throwRuntimeError(const std::string& note);
}

#endif 