#pragma once
#include "WindowContainer.h"

class Engine : public WindowContainer {
private:

public:
	Engine();
	bool Initialize(HINSTANCE instance, std::string title, int width, int height);
	bool HandleMessages();

	void Upadate();
	void RenderFrame();
};

