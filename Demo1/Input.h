#pragma once
#include "common1.h"

//��������libҪһ�𣬷����������GUID���޷���������
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "dinput.h"
#include <initguid.h>
#define ITEMS_NUM 10

class CInput
{
private:
	IDirect3DDevice9* m_pDevice;
	IDirectInput8* m_pInput;
	IDirectInputDevice8* m_pKey;
	IDirectInputDevice8* m_pMouse;
	HWND m_hWnd;

	char m_keyState[256];		//��ǰ����״̬
	char m_keyStateOld[256];	//��һ֡����״̬
	DIMOUSESTATE m_MouseState;
	DIMOUSESTATE m_MouseStateOld;

	POINT m_CursorPoint;
	
public:	
	CInput(IDirect3DDevice9* pDevice);
	~CInput(void);
	HRESULT Init(HINSTANCE hInst,HWND hWnd);
	void Update();
	BOOL KeyDown(int keyValue);
	BOOL KeyHold(int keyValue);
	BOOL KeyUp(int keyValue);

	BOOL MouseDown(int keyValue);
	BOOL MouseHold(int keyValue);
	BOOL MouseUp(int keyValue);

	LONG GetX(){return m_MouseState.lX;}
	LONG GetY(){return m_MouseState.lY;}
	LONG GetZ(){return m_MouseState.lZ;}

	HWND GetHWnd() {return m_hWnd;}
};
