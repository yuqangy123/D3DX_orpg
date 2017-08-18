#pragma once
#include "common1.h"

//以下两个lib要一起，否则输入相关GUID报无法解析错误
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

	char m_keyState[256];		//当前键盘状态
	char m_keyStateOld[256];	//上一帧键盘状态
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
