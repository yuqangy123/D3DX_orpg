#include "StdAfx.h"
#include "DemoGE.h"

CDemoGE::CDemoGE(void)
{
	m_pClassName=NULL;
	m_nFPS=0;
	m_nFPSPasstime = 0;
}

CDemoGE::~CDemoGE(void)
{
	DeleteAndNull(m_pClassName);
	CDXDevice::CloseInstance();
}

LRESULT CDemoGE::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_PAINT:{

				  }break;

	case WM_DESTROY:
		//		Cleanup();
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void CDemoGE::Update()
{
	m_pInput->Update();//input > camera
	m_pCamera->Update();//camera > scene
	m_pScene->Update(m_nFPSPasstime);
	
}

void CDemoGE::Render()
{
	Update();

	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW); 
	// 纹理采样
 	m_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
 	m_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	// Alpha测试
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF,0x000000ff);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_EQUAL);
// 	if (g_bFillMode)
// 		g_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
// 	else
// 		g_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	// 深度缓冲
	m_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	m_pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESS);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );//打开环境照明

	m_pCamera->AdjustCamera();
	m_pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);//可编程渲染管线要加上D3DCLEAR_ZBUFFER
	if(SUCCEEDED(m_pDevice->BeginScene()))
	{
		
		m_pScene->Render();

		wchar_t text[256]={0};
		D3DXVECTOR3 pos,look;
		m_pCamera->GetPosVtr(pos);
		m_pCamera->GetLookAtVtr(look);
		swprintf_s(text, _T("Camera: x=%f,y=%f,z=%f\r\nPos: x=%f,y=%f,z=%f\r\nFPS:%d"),pos.x,pos.y,pos.z,look.x,look.y,look.z, m_nFPS);
		RECT rect;
		rect.left=10;
		rect.top=20;
		rect.right=m_WinWidth;
		rect.bottom=150;
		m_pFont->DrawText(0,text,-1,&rect,DT_TOP|DT_LEFT,0xffffffff);

		m_pDevice->EndScene();
		m_pDevice->Present(NULL, NULL, NULL, NULL);
	}

	m_nFPSPasstime = m_FPS.GetFPS(m_nFPS);
}

void CDemoGE::SetupLights()
{
	// Set up a material. The material here just has the diffuse and ambient
	// colors set to yellow. Note that only one material can be used at a time.
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_pDevice->SetMaterial( &mtrl );
	//set up lights
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 0.80f;
	light.Diffuse.g  = 0.60f;
	light.Diffuse.b  = 0.60f;
	vecDir = D3DXVECTOR3(10,-10,-10);

	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	light.Range       = 5000.0f;
	m_pDevice->SetLight( 0, &light );
	m_pDevice->LightEnable( 0, TRUE );
	m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	// Finally, turn on some ambient light.
	m_pDevice->SetRenderState( D3DRS_AMBIENT, 0x00404040 );//为整个场景设置环境光
}

void CDemoGE::system_run()
{
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	m_FPS.start();
	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			Render();
		}
	}
	
	UnregisterClass(m_pClassName, m_hInst);
}

HRESULT CDemoGE::system_init(BOOL bWindow, int nWidth, int nHeight)
{
	LPCTSTR pClassName = L"D3D Demo_1";
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		pClassName, NULL };
	RegisterClassEx( &wc );


	HWND hWnd = CreateWindow( pClassName, L"D3D Demo_1 Name",
		WS_OVERLAPPEDWINDOW, 0, 0, nWidth,nHeight,//480,480 ,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	if(!hWnd)
		return E_FAIL;

	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	m_WinWidth = nWidth;m_WinHeight=nHeight;
	m_GHwnd = hWnd;
	m_pClassName = new wchar_t[32];
	wcscpy_s(m_pClassName, 32, pClassName);
	m_hInst = wc.hInstance;

	if(FAILED(CDXDevice::Instance()->InitD3DX(hWnd, bWindow, nWidth, nHeight)))
		return E_FAIL;

	if(FAILED(CDXDevice::Instance()->InitInput(m_hInst, hWnd)))
		return E_FAIL;

	if(FAILED(CDXDevice::Instance()->InitWavSound(hWnd)))
		return E_FAIL;

	//test code
	CSound CTestWav(_T("F:\\create.wav"));
	CTestWav.Play();

	if(FAILED(CDXDevice::Instance()->InitFont()))
		return E_FAIL;

	if(FAILED(CDXDevice::Instance()->InitCamera()))
		return E_FAIL;

	if(FAILED(CDXDevice::Instance()->InitScene()))
		return E_FAIL;

	m_pInput = CDXDevice::Instance()->GetD3DInput();
	m_pCamera = CDXDevice::Instance()->GetCamera();
	m_pScene = CDXDevice::Instance()->GetScene();	
	m_pDevice = CDXDevice::Instance()->GetD3Device();
	m_pFont = CDXDevice::Instance()->GetD3DFont();

	SetupLights();

	return S_OK;
}

			// //对应第一个顶点坐标和最后一个顶点坐标
			// int startX = - m_nWidth/ 2;
			// int startZ = m_nDepth / 2;
			// int endX = m_nWidth / 2;
			// int endZ = - m_nDepth / 2;
			// 
			// // 计算纹理u，v每格距离
			// float uCellSize = 1.0f / (float)m_nCol;
			// float vCellSize = 1.0f / (float)m_nRow;
			// LandVertex* pV;
			// if (FAILED(m_pVB->Lock(0,m_nVertexNum * sizeof(LandVertex),(void**)&pV,0)))
			// return false;
			// 
			// int i = 0;
			// for (int z = startZ;z >= endZ;z -= m_nCellSpace)
			// {
			// 	int j = 0;
			// 	for (int x = startX;x <= endX;x += m_nCellSpace)
			// 	{
			// 		int index = i * m_nCol + j;
			// 		pV[index].x = (float)x;
			// 		if (i != (m_nRow - 1) && j != (m_nCol - 1))
			// 		{
			// 			int heightIndex = i * (m_nCol - 1) + j;
			// 			pV[index].y = (float)m_pHeightmap[heightIndex];