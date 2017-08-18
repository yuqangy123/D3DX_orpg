#pragma once
// #include <Windows.h>
// #include <mmsystem.h>
#include "common1.h"

class CGWindows
{
public:
	CGWindows( );
	~CGWindows(void);
	HRESULT CreateGWindow(LPCTSTR lpClassName,
		LPCTSTR lpWindowName,
		DWORD dwStyle,
		int x,
		int y,
		int nWidth,
		int nHeight,
		HWND hWndParent,
		HMENU hMenu,
		HINSTANCE hInstance,
		LPVOID lpParam,
		WNDPROC WinProc);
	void RunModalLoop();


private:
	HWND m_GHwnd;
	WNDCLASSEX m_wc;
	wchar_t *m_pClassName;
};
