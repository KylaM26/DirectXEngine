#pragma once

#include <memory>
#include "RenderWindow.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class WindowContainer {
protected: 
	RenderWindow renderWindow;
	Graphics GFX;
	Keyboard keyboard;
	Mouse mouse;
private:
public:
	WindowContainer();
	LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
	//WindowProc
	//Mouse;
	//Keyboard;
};

