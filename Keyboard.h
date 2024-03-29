#pragma once

#include "KeyboardEvent.h"
#include <queue>

class Keyboard {
private:
	bool autoRepeatKeys;
	bool autoRepeatChars;
	bool keyStates[256];

	std::queue<KeyboardEvent> eventBuffer;
	std::queue<unsigned char> charBuffer;
public:
	Keyboard();
	
	bool KeyIsPressed(const unsigned char keycode);
	bool IsEventBufferEmpty();
	bool IsCharBufferEmpty();

	KeyboardEvent ReadKey();
	unsigned char ReadChar();

	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);

	void OnChar(const unsigned char key);

	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();

	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();

	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();
};

