#include "MouseEvent.h"

MouseEvent::MouseEvent() {
	this->type = EventType::Invalid;
}

MouseEvent::MouseEvent(const EventType type, const int x, const int y) {
	this->type = type;
	this->x = x;
	this->y = y;
}

bool MouseEvent::IsValid() const {
	return this->type != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const {
	return this->type;
}

MousePoint MouseEvent::GetPos() const {
	return MousePoint(this->x, this->y);
}

int MouseEvent::GetPositionX() const {
	return this->x;
}

int MouseEvent::GetPositionY() const {
	return this->y;
}
