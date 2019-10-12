#include "Engine.h"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmd, int code) {
	HRESULT result = CoInitialize(0);

	if (FAILED(result)) {
		Logger::Log(result, "Failed to co-initlize.");
		return 1;
	}

	Engine engine;

	if (engine.Initialize(instance, "IncredibleX", 800, 600)) {

		while (engine.HandleMessages()) {
			engine.Upadate();
			engine.RenderFrame();
		}

	}

	return 0;
}