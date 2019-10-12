#include "Logger.h"

void Logger::Log(std::string message) {
	MessageBoxA(NULL, message.c_str(), "Error", MB_OK);
}

void Logger::Log(HRESULT r, std::string message) {
	_com_error error(r);
	std::string errorMessage = "Error: " + message + "\n" + error.ErrorMessage();
	MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK);
}

void Logger::Log(HRESULT r, std::wstring message) {

}
