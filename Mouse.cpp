#include "Mouse.h"

void Mouse::OnLeftPressed(int x, int y) {
	this->isLeftDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void Mouse::OnLeftReleased(int x, int y) {
	this->isLeftDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void Mouse::OnRightPressed(int x, int y) {
	this->isRightDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void Mouse::OnRightReleased(int x, int y) {
	this->isRightDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void Mouse::OnMiddlePressed(int x, int y) {
	this->isMiddleButtonDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void Mouse::OnMiddleReleased(int x, int y) {
	this->isMiddleButtonDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void Mouse::OnWheelUp(int x, int y) {
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void Mouse::OnWheelDown(int x, int y) {
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void Mouse::OnMouseMove(int x, int y) {
	this->x = x, this->y = y;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void Mouse::OnMouseMoveRaw(int x, int y) {
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RawMove, x, y));
}

bool Mouse::IsLeftDown() {
	return this->isLeftDown;
}

bool Mouse::IsMiddleDown() {
	return this->isMiddleButtonDown;
}

bool Mouse::IsRightDown() {
	return this->isRightDown;
}

int Mouse::GetPositionX() {
	return this->x;
}

int Mouse::GetPositionY() {
	return this->y;
}

MousePoint Mouse::GetPosition() {
	return MousePoint(this->x, this->y);
}

bool Mouse::IsEventBufferEmpty() {
	return eventBuffer.empty();
}

MouseEvent Mouse::ReadEvent() {
	if (!this->eventBuffer.empty()) {
		MouseEvent e = eventBuffer.front();
		eventBuffer.pop();
		return e;
	} return MouseEvent(MouseEvent::EventType::Invalid, 0, 0);
}
