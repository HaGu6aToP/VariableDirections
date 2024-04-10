#pragma once
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <process.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


#include <sstream>
#include <string>
#include <vector>

#include "Graphics.h"
#include "Methods.h"
#include "Array.h"

#define _USE_MATH_DEFINES
#include <math.h>
#pragma comment(linker, "/entry:WinMainCRTStartup")

void thread(void* pParams);
void setOpenFileParams(HWND hWnd);
void loadData(LPCSTR path);
void myMenu(HWND hWnd);
void ConstantWndAddWidgets(HWND hWnd);
static std::pair<bool, HWND> addWindow(const std::wstring&& winClass, const std::wstring&& title, HWND hParrentHwnd, const WNDPROC callback);
void show();