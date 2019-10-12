#include "WindowContainer.h"

WindowContainer::WindowContainer() {
	// For
	static bool rawInputIsInitialized = false;

	if (!rawInputIsInitialized) {
		RAWINPUTDEVICE rid = { 0 };

		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
			Logger::Log("Failed to register raw input device.");
			exit(-1);
		}

		rawInputIsInitialized = true;
	}
}

LRESULT  WindowContainer::WindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_CHAR: {
		unsigned char keycode = static_cast<unsigned char>(wParam);

		if (keyboard.IsCharsAutoRepeat()) {
			keyboard.OnChar(keycode);
		}
		else {  // Since this is when auto repeat is off
		 // Check if it is a auto repeat key.
			const bool wasPressed = lParam & 0x40000000;

			if (!wasPressed) {
				keyboard.OnChar(keycode);
			}
		}

		return 0;
	}

	case WM_KEYDOWN: {
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (keyboard.IsKeysAutoRepeat()) {
			keyboard.OnKeyPressed(keycode);
		}
		else {
			const bool wasPressed = lParam & 0x40000000;

			if (!wasPressed) {
				keyboard.OnChar(keycode);
			}
		}
		return 0;
	}

	case WM_KEYUP: {
		unsigned char keycode = static_cast<unsigned char>(wParam);
		keyboard.OnKeyReleased(keycode);
		return 0;
	}

	case WM_MOUSEMOVE: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMouseMove(x, y);
		return 0;
	}

	case WM_LBUTTONDOWN: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x, y);
		return 0;
	}

	case WM_LBUTTONUP: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x, y);
		return 0;
	}

	case WM_RBUTTONDOWN: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x, y);
		return 0;
	}

	case WM_RBUTTONUP: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x, y);
		return 0;
	}

	case WM_MBUTTONDOWN: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddlePressed(x, y); 
		return 0;
	}

	case WM_MBUTTONUP: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddleReleased(x, y);
		return 0;
	}

	case WM_MOUSEWHEEL: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) // Scrolling up
			mouse.OnWheelUp(x, y);
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) // Scrolling down
			mouse.OnWheelDown(x, y);
	} break;

	case WM_INPUT: { // For RAW mouse movement
		// 1.) Register raw input devices(the mouse)
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, 0, &dataSize, sizeof(RAWINPUTHEADER));

		if (dataSize > 0) {
			std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);

			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());

				if (raw->header.dwType == RIM_TYPEMOUSE) {
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

		return DefWindowProc(handle, msg, wParam, lParam);
	}
	default:
		return DefWindowProc(handle, msg, wParam, lParam);
	}


}