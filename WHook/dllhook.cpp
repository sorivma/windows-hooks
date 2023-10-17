#include "dllhook.h"
#include "atltypes.h"
#include "cmath"
#include <vector>
#include <string>
#include <ctime>
#include <mmsystem.h>
#include <map>
#include <iostream>
#include <fstream>
#pragma comment(lib, "Winmm.lib")

HHOOK hMouseHook;
HHOOK hKeyboardHook;
HINSTANCE hInst;


EXPORT void CALLBACK SetMouseHook(void)
{
	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInst, 0);
}

EXPORT void CALLBACK UnMouseHook(void)
{
	UnhookWindowsHookEx(hMouseHook);
}

EXPORT void CALLBACK SetKeyBoardHook(void)
{
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInst, 0);
}

EXPORT void CALLBACK UnKeyBoardHook(void)
{
	PlaySound(NULL, NULL, 0);
	UnhookWindowsHookEx(hKeyboardHook);
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0) {
		CallNextHookEx(hKeyboardHook, code, wParam, lParam);
	}

	if (wParam == WM_KEYDOWN) {
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);
		switch (p -> vkCode)
		{
		case VK_CAPITAL:
			if (!(GetKeyState(VK_CAPITAL) & 1))
			{
				PlaySoundW(L"D:\\alarm_1.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			else
			{
				PlaySound(NULL, 0, 0);
			}
			break;
		case VK_NUMLOCK:
			if (!(GetKeyState(VK_NUMLOCK) & 1))
			{
				PlaySoundW(L"D:\\alarm_2.mav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			else
			{
				PlaySound(NULL, 0, 0);
			}
			break;
		default:
			break;
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}

const int SCREEN_WIDTH = 1980;   // Adjust the screen dimensions as needed
const int SCREEN_HEIGHT = 1200;


COLORREF GetRandomColor(int x, int y) {
	return RGB(rand() % 256, rand() % 256, rand() % 256);
}

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0 && wParam == WM_MOUSEMOVE)
	{
		MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

		// Generate a random color for the square
		COLORREF squareColor = GetRandomColor(pMouseStruct->pt.x, pMouseStruct->pt.y);

	    int SQUARE_SIZE = 30;
		// Calculate the square's coordinates
		int left = pMouseStruct->pt.x + SQUARE_SIZE / 2 + rand() % 200; 
		int top = pMouseStruct->pt.y + SQUARE_SIZE / 2 + rand() % 200;
		int right = left - SQUARE_SIZE + rand() % 200;
		int bottom = top - SQUARE_SIZE + rand() % 200;

		// Get the device context for the entire screen
		HDC screenDC = GetDC(NULL);

		// Create a brush with the specified color
		HBRUSH hBrush = CreateSolidBrush(squareColor);

		RECT rect = { left, top, right, bottom };
		FillRect(screenDC, &rect, hBrush);

		// Delete the brush
		DeleteObject(hBrush);

		// Release the device context
		ReleaseDC(NULL, screenDC);
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);


	hInst = hinstDLL;
	return TRUE;
}
