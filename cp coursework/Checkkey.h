#pragma once
#include "windows.h"

class Checkkey
{
public:
	bool keyPressed(int key)
	{
		return GetAsyncKeyState(key) & 0x8000; 
	}
};