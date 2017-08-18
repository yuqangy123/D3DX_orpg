#include "stdafx.h"
#include "Input.h"

CInput::CInput(IDirect3DDevice9* pDevice)
{
	m_pDevice = pDevice;
	m_pInput = NULL;
	m_pKey = NULL;
	m_pMouse = NULL;

	ZeroMemory(m_keyState,256);
	ZeroMemory(m_keyStateOld,256);
}

CInput::~CInput(void)
{
	if(m_pKey)
		m_pKey->Unacquire();

	if(m_pMouse)
		m_pMouse->Unacquire();
	

	SafeRelease(m_pKey);
	SafeRelease(m_pMouse);
	SafeRelease(m_pInput);
}

HRESULT CInput::Init(HINSTANCE hInst,HWND hWnd)
{
	m_hWnd = hWnd;
	if (DI_OK != DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(LPVOID*)&m_pInput,NULL)){
		MessageBox(hWnd,_T("����Input����ʧ��!"),_T("ERROR"),MB_OK);
		return E_FAIL;
	}

	// ���������豸
	if (DI_OK != m_pInput->CreateDevice(GUID_SysKeyboard,&m_pKey,NULL)){
		MessageBox(hWnd,_T("���������豸ʧ��!"),_T("ERROR"),MB_ICONERROR | MB_OK);
		return E_FAIL;
	}

	// ���ü������ݸ�ʽΪ��׼���̶���
	if (DI_OK != m_pKey->SetDataFormat(&c_dfDIKeyboard)){
		MessageBox(hWnd,_T("�����������ݸ�ʽʧ��!"),_T("ERROR"),MB_OK);
		return E_FAIL;
	}

	// ���ü��̵�Э���㼶
	m_pKey->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	// ��ȡ����Ȩ
	m_pKey->Acquire();

	// ��������豸
	if (DI_OK != m_pInput->CreateDevice(GUID_SysMouse,&m_pMouse,NULL)){
		MessageBox(hWnd,_T("��������豸ʧ��!"),_T("ERROR"),MB_ICONERROR | MB_OK);
		return E_FAIL;
	}

	// ����������ݸ�ʽΪ��׼������
	if (DI_OK != m_pMouse->SetDataFormat(&c_dfDIMouse)){
		MessageBox(hWnd,_T("����������ݸ�ʽʧ��!"),_T("ERROR"),MB_OK);
		return E_FAIL;
	}

	// ��������Э���㼶
	m_pMouse->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	// ��ȡ����Ȩ
	m_pMouse->Acquire();

	RECT winRt;
	GetWindowRect(hWnd, &winRt);
	m_CursorPoint.x = (winRt.right-winRt.left)/2;
	m_CursorPoint.y = (winRt.bottom-winRt.top)/2;
	return S_OK;
}

void CInput::Update()
{
	memcpy(m_keyStateOld,m_keyState,256);
	if(DI_OK != m_pKey->GetDeviceState(sizeof(m_keyState),m_keyState))
	{
		memset(m_keyState,0,sizeof(m_keyState));
		m_pKey->Acquire();
	}

	memcpy(&m_MouseStateOld,&m_MouseState,sizeof(m_MouseState));
	if(DI_OK != m_pMouse->GetDeviceState(sizeof(m_MouseState),&m_MouseState))
	{
		memset(&m_MouseState,0,sizeof(m_MouseState));
		m_pMouse->Acquire();
	}

//	SetCursorPos(m_CursorPoint.x, m_CursorPoint.y);
}

BOOL CInput::KeyDown(int keyValue)
{
	return ( !(m_keyStateOld[keyValue] & 0x80) && (m_keyState[keyValue] & 0x80) );
}


BOOL CInput::KeyHold(int keyValue)
{
	return ( (m_keyStateOld[keyValue] & 0x80) && (m_keyState[keyValue] & 0x80) );
}


BOOL CInput::KeyUp(int keyValue)
{
	return ( (m_keyStateOld[keyValue] & 0x80) && !(m_keyState[keyValue] & 0x80) );
}

BOOL CInput::MouseDown(int keyValue)
{
	return ( (m_MouseState.rgbButtons[keyValue] & 0x80) && !(m_MouseStateOld.rgbButtons[keyValue] & 0x80) );
}


BOOL CInput::MouseHold(int keyValue)
{
	return ( (m_MouseState.rgbButtons[keyValue] & 0x80) && (m_MouseStateOld.rgbButtons[keyValue] & 0x80) );
}


BOOL CInput::MouseUp(int keyValue)
{
	return ( !(m_MouseState.rgbButtons[keyValue] & 0x80) && (m_MouseStateOld.rgbButtons[keyValue] & 0x80) );
}

