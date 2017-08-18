#include "StdAfx.h"
#include "Scene.h"
#include "ResFileManager.h"
#include "DXDevice.h"

CScene::CScene(IDirect3DDevice9* p):m_pDevice(p)
{
	m_pMeshManage=NULL;
	m_pDevice=p;
	m_pTerrain=NULL;
	m_pFrustum=NULL;
	m_pFont=NULL;
	m_pMajor=NULL;
	m_pSky=NULL;
	m_pSnow=NULL;
}

CScene::~CScene(void)
{
	DeleteAndNull(m_pMeshManage);
 	DeleteAndNull(m_pTerrain);
	DeleteAndNull(m_pFrustum);
	DeleteAndNull(m_pMajor);
	DeleteAndNull(m_pSky);
	DeleteAndNull(m_pSnow);
}

HRESULT CScene::Init(CCamera *pCamera)
{
	m_pCamera = pCamera;
	
	m_pMeshManage = new CMeshManager;
	m_pFrustum = new CZFrustum;
	m_pTerrain = new CTerrain(m_pDevice);
	m_pMajor = new CMajorObject;
	m_pSky = new CSky;
	m_pDeck = new CDeck;
	m_pSnow = new CSnow(m_pDevice);

	if(!m_pFrustum || !m_pMeshManage || !m_pTerrain || !m_pMajor || !m_pSky || !m_pSnow)
		return E_FAIL;

	if(FAILED(m_pMeshManage->LoadMeshFile()))
		return E_FAIL;

	if(FAILED(m_pTerrain->Init(m_pFrustum)))
		return E_FAIL;

	if(FAILED(m_pMajor->Init()))
		return E_FAIL;

	if(FAILED(m_pSky->Init()))
		return E_FAIL;

	if(FAILED(m_pDeck->Init()))
		return E_FAIL;

	if(FAILED(m_pSnow->Init(CResFileManager::Instance()->GetSnowTextureFile())))
		return E_FAIL;

	if(NULL == (m_pFont = CDXDevice::Instance()->GetD3DFont()))
		return E_FAIL;

	return S_OK;	
}

// float CScene::GetTHeight(float x, float z)
// {	
// 	return m_pTerrain->GetTHeight(x,z);
// }

void CScene::Update(double nFpsTime)
{
	D3DXMATRIX matView;//视图变换矩阵
	D3DXMATRIX matProj;//平截台体的投影矩阵
	D3DXMATRIX matZFrustum;
 	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
 	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	matZFrustum=matView*matProj;

	m_pFrustum->BuildFrustum(&matZFrustum, m_pTerrain->GetCellSpace());
	
	D3DXVECTOR3 vDir, vCamera, vCLookat;
	m_pCamera->GetPosVtr(vCamera);
	m_pCamera->GetLookAtVtr(vCLookat);
	vDir = vCLookat-vCamera;//求得方向

	m_pSky->Update(vCLookat);
	m_pMajor->Update(nFpsTime, vDir);
	m_pDeck->Update();
	m_pTerrain->Update();
	m_pSnow->Update(nFpsTime);
}

void CScene::Render()//注意Update和Render顺序
{
//	m_pMeshManage->Render();
	m_pSky->Render();
	m_pMajor->Render();
	m_pDeck->Render();
	m_pTerrain->Render();	
	m_pSnow->Render();
//	m_pFrustum->Render(m_pDevice);	


	wchar_t text[256]={0};
	swprintf_s(text, _T("地形三角形数量:%d, 共%d\r\n"),m_pTerrain->GetRenderTriNum());
	RECT rect;
	rect.left=10;
	rect.top=5;
	rect.right=500;
	rect.bottom=100;
	m_pFont->DrawText(0,text,-1,&rect,DT_TOP|DT_LEFT,0xffffffff);
	
}

CTerrain* CScene::GetCurTerrain()
{
	return m_pTerrain;
}
