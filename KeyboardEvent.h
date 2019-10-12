#pragma once

class KeyboardEvent {
public:
	enum EventType {
		Press,
		Release,
		Invalid
	};
private:
	EventType type;
	unsigned char key;
public:
	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	
	bool IsPressed() const;
	bool IsReleased() const;
	bool IsValid() const;

	unsigned char GetKeyCode() const;
};

