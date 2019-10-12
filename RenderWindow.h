#pragma once
#include "Logger.h"

class WindowContainer;

class RenderWindow {
private:
	HWND handle;
	HINSTANCE instance;
	std::string title;
	int width, height;
public:
	bool Initialize(WindowContainer* container, HINSTANCE instance, std::string title, int width, int height);
	bool HandleMessages();
	HWND GetHandle() const;
	~RenderWindow();
};

