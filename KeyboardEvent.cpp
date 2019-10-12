#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() {
	this->type = EventType::Invalid;
	this->key = 0u;
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key) {
	this->type = type;
	this->key = key;
}

bool KeyboardEvent::IsPressed() const {
	return this->type == EventType::Press;
}

bool KeyboardEvent::IsReleased() const {
	return this->type == EventType::Release;
}

bool KeyboardEvent::IsValid() const {
	return this->type != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const {
	return key;
}
