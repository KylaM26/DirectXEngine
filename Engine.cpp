#include "Engine.h"

Engine::Engine() {
}

bool Engine::Initialize(HINSTANCE instance, std::string title, int width, int height) {
	if (!this->renderWindow.Initialize(this, instance, title, width, height))
		return false;

	if (!GFX.Initialize(this->renderWindow.GetHandle(), width, height)) 
		return false;
	
	return true;
}

bool Engine::HandleMessages() {
	return this->renderWindow.HandleMessages();
}

void Engine::Upadate() {
	while (!keyboard.IsCharBufferEmpty()) {
		unsigned char o1 = keyboard.ReadChar();
		//std::string msg1 = "Char: ";
		//msg1 += o1;
		//msg1 += "\n";
		//OutputDebugStringA(msg1.c_str());
	}

	while (!keyboard.IsEventBufferEmpty()) {
		unsigned char o2 = keyboard.ReadChar();
		//std::string msg2 = "Char: ";
		//msg2 += o2;
		//msg2 += "\n";
		//OutputDebugStringA(msg2.c_str());

	}

//	while (!mouse.IsEventBufferEmpty()) {
	/*	MouseEvent me = mouse.ReadEvent();

		if (me.GetType() == MouseEvent::EventType::WheelUp)
			OutputDebugStringA("Wheel up");
		else if (me.GetType() == MouseEvent::EventType::WheelDown)
			OutputDebugStringA("Wheel down");

		if (me.GetType() == MouseEvent::EventType::RawMove) {
			std::string o3 = "X: ";		
			o3 += std::to_string(me.GetPositionX());
o3 += ", Y: ";
o3 += std::to_string(me.GetPositionY());
o3 += "\n";
OutputDebugStringA(o3.c_str());
		}*/

		//std::string o3 = "X: ";
		//o3 += std::to_string(me.GetPositionX());
		//o3 += ", Y: ";
		//o3 += std::to_string(me.GetPositionY());
		//o3 += "\n";
		//OutputDebugStringA(o3.c_str());
//	}
}

void Engine::RenderFrame() {
	GFX.RenderFrame();
}
