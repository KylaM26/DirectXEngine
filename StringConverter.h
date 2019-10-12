#pragma once

#include <string>

class StringConverter {
private:
	StringConverter();
	StringConverter(const StringConverter&);
	const StringConverter& operator=(const StringConverter&) {}
public:
	static std::wstring StringToWide(std::string str);
};

