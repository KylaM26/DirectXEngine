#include "WindowContainer.h"

LRESULT CALLBACK WindowContainerProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(handle);
		return 0;
		
	default: {
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(handle, GWLP_USERDATA));
		return pWindow->WindowProc(handle, msg, wParam, lParam);
	}
			 
	}
}

// This handles the setup then it redirects it to the windowContainerProc.
LRESULT CALLBACK HandleMessageSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_NCCREATE: { // When window is created
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam); 
		WindowContainer* windowContainer = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

		if (!windowContainer) {
			Logger::Log("Critical error: Pointer to window is null during WN_NCCREATE.");
			exit(1);
		}

		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowContainer));
		SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowContainerProc));
	
		return windowContainer->WindowProc(handle, msg, wParam, lParam);
	}

	default: 
		return DefWindowProc(handle, msg, wParam, lParam);
	}
}

bool RenderWindow::Initialize(WindowContainer* container, HINSTANCE instance, std::string title, int width, int height) {
	WNDCLASSA windowClass = { 0 };

	this->instance = instance;
	this->title = title;
	this->width = width, this->height = height;

	windowClass.hInstance = instance;
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpszClassName = this->title.c_str();
	windowClass.lpfnWndProc = HandleMessageSetup;

	if (!RegisterClassA(&windowClass)) {
		Logger::Log("Failed to register window class.");
		return false;
	}

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->height / 2;

	RECT windowRect = { 0 };
	windowRect.left = centerScreenX;
	windowRect.top = centerScreenY;
	windowRect.right = windowRect.left + this->width;
	windowRect.bottom = windowRect.top + this->height;

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);


	handle = CreateWindowA(windowClass.lpszClassName, this->title.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 0, 0, instance, container);

	if (!handle) {
		Logger::Log("Failed to create window");
		return false;
	}

	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::HandleMessages() {
	MSG message = { 0 };

	while (PeekMessageA(&message, this->handle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// Handling here so it will close this window only(In case of multiple windows)
	if (message.message == WM_NULL) {
		if (!IsWindow(this->handle)) {
			this->handle = 0;
			UnregisterClass(this->title.c_str(), this->instance);
			return false;
		}
	} return true;
}

HWND RenderWindow::GetHandle() const {
	return this->handle;
}

RenderWindow::~RenderWindow() {
	if (this->handle) {
		UnregisterClass(this->title.c_str(), this->instance);
		DestroyWindow(this->handle);
	}
}
