#include "StdAfx.h"
#include "GWindows.h"

CGWindows::CGWindows(void)
{
	m_GHwnd = NULL;
	m_pClassName = NULL;
}

CGWindows::~CGWindows(void)
{
	if(m_pClassName){
		UnregisterClass( m_pClassName, m_wc.hInstance );
		DeleteAndNull(m_pClassName);
	}
}


void CGWindows::RunModalLoop()
{
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );
		while( msg.message!=WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage(&msg);
				DispatchMessage( &msg );
			}
		}
		
}

HRESULT CGWindows::CreateGWindow(LPCTSTR lpClassName,
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
					  WNDPROC WinProc)
{
	
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WinProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		lpClassName, NULL };
	RegisterClassEx( &wc );
	memcpy_s(&m_wc, sizeof(wc), &wc, sizeof(wc));

	HWND hWnd = CreateWindow( lpClassName, lpWindowName,
		dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	if(!hWnd)
		return E_FAIL;
	m_GHwnd = hWnd;
	m_pClassName = new wchar_t[32];
	wcscpy_s(m_pClassName, 32, lpClassName);

	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	return S_OK;
}

LRESULT CALLBACK CallWindowProc(          HWND hwnd,
								  UINT uMsg,
								  WPARAM wParam,
								  LPARAM lParam
					   )
{
	return S_OK;
}