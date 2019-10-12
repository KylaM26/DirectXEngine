#pragma once

#include "StringConverter.h"
#include <comdef.h>
#include <Windows.h>

class Logger {
private:
	Logger();
	Logger(const Logger&);
	const Logger& operator=(const Logger&) {}
public:
	static void Log(std::string message);
	static void Log(HRESULT r, std::string message);
	static void Log(HRESULT r, std::wstring message);
};

