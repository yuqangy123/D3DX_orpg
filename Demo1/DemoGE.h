#pragma once
//#include <Windows.h>
//#include <mmsystem.h>
#include "DXDevice.h"
#include "FPSCounter.h"

class CInput;
class CCamera;
class CScene;
class CDemoGE
{
public:
	CDemoGE(void);
	~CDemoGE(void);
	HRESULT system_init(BOOL bWindow, int nWidth, int nHeight);
	void system_run();
	

protected:
	static LRESULT CALLBACK MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Render();
	void SetupLights();
	void Update();
	

private:
	HINSTANCE		m_hInst;					// Ó¦ÓÃ³ÌÐò¾ä±ú
	HWND		m_GHwnd;
	wchar_t		*m_pClassName;

	CInput			*m_pInput;
	CCamera		*m_pCamera;
	CScene			*m_pScene;
	ID3DXFont		*m_pFont;
	IDirect3DDevice9*		m_pDevice;
	int				m_WinWidth;
	int				m_WinHeight;

	CFPSCounter		m_FPS;
	int						m_nFPS;
	double				m_nFPSPasstime;
};
