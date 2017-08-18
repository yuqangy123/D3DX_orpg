#include "StdAfx.h"
#include "DXDevice.h"


CDXDevice* CDXDevice::g_pOwner = NULL;
CDXDevice::CDXDevice(void)
{
	m_Mult = D3DMULTISAMPLE_NONE;
	m_pInput=NULL;
	m_pCamera=NULL;
	m_pScene=NULL;
	m_pFont=NULL;
}

CDXDevice::~CDXDevice(void)
{
	DeleteAndNull(m_pInput);
	DeleteAndNull(m_pCamera);
	DeleteAndNull(m_pScene);
	SafeRelease(m_pFont);
	SafeRelease(m_pDevice);
}

void CDXDevice::GetLastErr()
{
	TCHAR* buffer;
	DWORD error = ERROR_DS_OBJ_STRING_NAME_EXISTS;
	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		error,
		0,
		( LPTSTR )&buffer,
		0,
		NULL );
	::MessageBoxA( NULL, (LPCSTR)buffer, "error", MB_OK );
	LocalFree( buffer );  
}


HRESULT CDXDevice::InitD3DX(HWND hMainHwnd, BOOL bWindow, int nWidth, int nHeight, BOOL bZbuffer/*=TRUE*/, BOOL bMultsample/*=TRUE*/)
{
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;
	
	// 获取显卡设备的显示模式
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm)))
		return E_FAIL;


	D3DPRESENT_PARAMETERS d3dpp;// Direct3D设备信息
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed = bWindow;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferWidth = nWidth;//d3ddm.Width;
	d3dpp.BackBufferHeight = nHeight;//d3ddm.Height;
 	d3dpp.hDeviceWindow = hMainHwnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
 	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
 	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		//选择深度缓冲格式, 注意Clear的时候要选择清楚深度缓冲D3DCLEAR_ZBUFFER
		if (m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
			d3ddm.Format,D3DUSAGE_DEPTHSTENCIL,
			D3DRTYPE_SURFACE,D3DFMT_D32) == D3D_OK)
			// 选择32位深度缓冲区
		{
			d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
		}
		else if (m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
			d3ddm.Format,D3DUSAGE_DEPTHSTENCIL,
			D3DRTYPE_SURFACE,D3DFMT_D24X8) == D3D_OK)
			// 选择24位深度缓冲区
		{
			d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8/*D3DFMT_D16*/;
		}
		else if (m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
			d3ddm.Format,D3DUSAGE_DEPTHSTENCIL,
			D3DRTYPE_SURFACE,D3DFMT_D16) == D3D_OK)
			// 选择16位深度缓冲区
		{
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		}
		else 
		{
			d3dpp.EnableAutoDepthStencil = FALSE;
		}
		if(bZbuffer && d3dpp.AutoDepthStencilFormat)
			d3dpp.EnableAutoDepthStencil = TRUE;

	/*
	//bug, 不同电脑呈现的图像尺寸不同
		// 图形反锯齿
		for (int i=16;i>1;i--)
		{
			if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3ddm.Format,
				FALSE,(D3DMULTISAMPLE_TYPE)i,NULL)))
			{
				m_Mult = (D3DMULTISAMPLE_TYPE)i;
				break;
			}
			if (i == 1)
			{
				m_Mult = (D3DMULTISAMPLE_TYPE)0;
			}
		}
		if (bMultsample == TRUE && m_Mult != 0)//默认开启多重采样
		{
			d3dpp.MultiSampleType = m_Mult;
		}
*/
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hMainHwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&m_pDevice)))
	{
		GetLastErr();
		return E_FAIL;
	}
	
	//m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_winHeight = nHeight;
	m_winWidth = nWidth;
	m_bWindow = bWindow;
	m_hMainHwnd = hMainHwnd;
	return NewObj();
}

HRESULT CDXDevice::NewObj()
{
	m_pInput = new CInput(m_pDevice);
	m_pScene = new CScene(m_pDevice);
	m_pCamera = new CCamera(m_pDevice);

	if(!m_pInput || !m_pScene || !m_pCamera)
		return E_FAIL;

	return S_OK;
}


HRESULT CDXDevice::InitInput(HINSTANCE hInstance, HWND hWnd )
{
	if(!m_pDevice)
		return E_FAIL;

	if(FAILED(m_pInput->Init(hInstance, hWnd)))
		return E_FAIL;

	return S_OK;
}
HRESULT CDXDevice::InitCamera()
{
	if(!m_pDevice || !m_pInput || !m_pScene)
		return E_FAIL;
	
	if(FAILED(m_pCamera->InitCamera(m_pScene, m_pInput, m_winWidth, m_winHeight)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDXDevice::InitScene()
{
	if(!m_pDevice || !m_pCamera)
		return E_FAIL;
	
	if(FAILED(m_pScene->Init(m_pCamera)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CDXDevice::InitFont()
{
	if(!m_pDevice)
		return E_FAIL;

	D3DXCreateFont(m_pDevice,12,12,400,0,false,GB2312_CHARSET,OUT_TT_ONLY_PRECIS,DEFAULT_QUALITY,0,_T("宋体"),&m_pFont);

	return S_OK;
}

HRESULT CDXDevice::InitWavSound(HWND hWnd)
{
	HRESULT hr;
	hr = CSoundLayer::CreateInstance(hWnd);
	return hr;
}