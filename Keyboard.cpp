#include "Keyboard.h"

Keyboard::Keyboard() {
	for (int i = 0; i < sizeof(this->keyStates); i++) {
		this->keyStates[i] = false;
	}

	this->autoRepeatChars = false, this->autoRepeatKeys = false;
	
}

bool Keyboard::KeyIsPressed(const unsigned char keycode) {
	return keyStates[keycode];
}

bool Keyboard::IsEventBufferEmpty() {
	return eventBuffer.empty();
}

bool Keyboard::IsCharBufferEmpty() {
	return charBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey() {
	if (this->eventBuffer.empty()) {
		return KeyboardEvent();
	}

	KeyboardEvent e = this->eventBuffer.front(); // Storing the front event.
	this->eventBuffer.pop(); // Removing the front event
	return e;
}

unsigned char Keyboard::ReadChar() {
	if (this->charBuffer.empty()) {
		return 0u;
	}

	unsigned char c = this->charBuffer.front();
	this->charBuffer.pop();
	return c;
}

void Keyboard::OnKeyPressed(const unsigned char key) {
	this->eventBuffer.push((KeyboardEvent(KeyboardEvent::EventType::Press, key)));
}

void Keyboard::OnKeyReleased(const unsigned char key) {
	this->eventBuffer.push((KeyboardEvent(KeyboardEvent::EventType::Release, key)));
}

void Keyboard::OnChar(const unsigned char key) {
	this->charBuffer.push(key);
}

void Keyboard::EnableAutoRepeatKeys() {
	this->autoRepeatKeys = true;
}

void Keyboard::DisableAutoRepeatKeys() {
	this->autoRepeatKeys = false;
}

void Keyboard::EnableAutoRepeatChars() {
	this->autoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatChars() {
	this->autoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat() {
	return this->autoRepeatKeys;
}


bool Keyboard::IsCharsAutoRepeat() {
	return this->autoRepeatChars;
}
