#pragma once
#include "MouseEvent.h"
#include <queue>

class Mouse {
private:
	std::queue<MouseEvent> eventBuffer;

	bool isLeftDown;
	bool isRightDown;
	bool isMiddleButtonDown;

	int x, y;
public:
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);

	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);

	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);

	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);

	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	int GetPositionX();
	int GetPositionY();

	MousePoint GetPosition();

	bool IsEventBufferEmpty();
	MouseEvent ReadEvent();
};

