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

class Checkmouse {
public:
    POINT lastCursorPos; 
    HWND hwnd;          

    Checkmouse(HWND windowHandle) : hwnd(windowHandle)
    {
        GetCursorPos(&lastCursorPos);
        ScreenToClient(hwnd, &lastCursorPos);
    }

    void getMouseMovement(float& mouseX, float& mouseY) 
    {
        POINT currentCursorPos;
        GetCursorPos(&currentCursorPos);
        ScreenToClient(hwnd, &currentCursorPos);

        mouseX = static_cast<float>(currentCursorPos.x - lastCursorPos.x);
        mouseY = static_cast<float>(currentCursorPos.y - lastCursorPos.y);

        lastCursorPos = currentCursorPos;
        const int threshold = 200;
        Vec2 center = Vec2(500, 300);
        POINT screenCenter = { static_cast<LONG>(center.x), static_cast<LONG>(center.y) };
       if (abs(currentCursorPos.x - center.x) > threshold || abs(currentCursorPos.y - center.y) > threshold)
       {
            SetCursorPos(center.x, center.y);
            lastCursorPos = screenCenter;
            mouseX = 0.0f;
            mouseY = 0.0f;
            return;
        }
       lastCursorPos = currentCursorPos;
       ShowCursor(FALSE);
    }
};
