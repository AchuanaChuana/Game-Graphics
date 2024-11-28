#pragma once
#include <windows.h>
#include <string>
#include "memory.h"
#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))


class Window
{
public:
	HWND hwnd;
	HINSTANCE hinstance;
	std::string name;
	int width;
	int height;
	DWORD style;
	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];


	void Init(int window_width, int window_height, const std::string window_name, int window_x = 0, int window_y = 0);
	void updateMouse(int x, int y);
	void processMessages();
};