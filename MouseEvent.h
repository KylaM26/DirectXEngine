#pragma once

struct MousePoint {
	int x, y;
	MousePoint(int x, int y): x(x), y(y) {}
};

class MouseEvent {
public:
	enum EventType {
		LPress,
		LRelease,

		RPress,
		RRelease,

		MPress,
		MRelease,

		WheelUp,
		WheelDown,

		Move, 
		RawMove,

		Invalid
	};
private:
	EventType type;
	int x, y;
public:
	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);

	bool IsValid() const;

	EventType GetType() const;
	MousePoint GetPos() const;

	int GetPositionX() const;
	int GetPositionY() const;
};

