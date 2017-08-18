#pragma once
#include "common1.h"
#include "Camera.h"
#include "Scene.h"
#include "Terrain.h"
#include "Input.h"
#include "Sound.h"
#include "ResFileManager.h"

class CCamera;
class CTerrain;
class CScene;
class CInput;
class CSound;

class CDXDevice
{
public:
	static CDXDevice* Instance(){NewAndReturn(g_pOwner, CDXDevice)};
	static void CloseInstance(){DeleteAndNull(g_pOwner);};
	void GetLastErr();

	HRESULT InitD3DX(HWND hMainHwnd, BOOL bWindow, int nWidth, int nHeight, BOOL bZbuffer=TRUE, BOOL bMultsample=TRUE);
	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
	HRESULT InitCamera();
	HRESULT InitScene();
	HRESULT InitFont();
	HRESULT InitWavSound(HWND hWnd);

	//�����л���ʱ��ָ��Ҳ��Ҫ����
	IDirect3DDevice9* GetD3Device(){return m_pDevice;};
	CInput* GetD3DInput(){return m_pInput;};
	CCamera*	GetCamera(){return m_pCamera;};
	CScene* GetScene(){return m_pScene;};//�Ż�������ж������
	ID3DXFont* GetD3DFont(){return m_pFont;};
	HWND GetMainHwnd(){return m_hMainHwnd;};

protected:
	HRESULT NewObj();

private:
	CDXDevice(void);
	~CDXDevice(void);
	static CDXDevice *g_pOwner;

	CInput			*m_pInput;
	CCamera		*m_pCamera;
	CScene			*m_pScene;
	ID3DXFont		*m_pFont;
	HWND			m_hMainHwnd;

	IDirect3D9*				m_pD3D;				// D3Dָ��
	IDirect3DDevice9*		m_pDevice;			// D3D�豸ָ��
	D3DMULTISAMPLE_TYPE m_Mult;	// ���֧�ֶ��ز�����

	BOOL			m_bWindow;
	int				m_winWidth;
	int				m_winHeight;
};
