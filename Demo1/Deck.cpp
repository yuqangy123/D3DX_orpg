#include "StdAfx.h"
#include "Deck.h"
#include "DXDevice.h"
#include "Terrain.h"

CDeck::CDeck(void)
{
	m_pMesh = NULL;
}

CDeck::~CDeck(void)
{
	DeleteAndNull(m_pMesh);
}

HRESULT CDeck::Init()
{
	m_pDevice = CDXDevice::Instance()->GetD3Device();
	m_pMesh = new CMesh(m_pDevice);

	string_t csMeshfile = CResFileManager::Instance()->GetMeshFilePath();
	csMeshfile.append(_T("\\car.x"));
	string_t csTexturePath = CResFileManager::Instance()->GetTextureFilePath();
	if(FAILED(m_pMesh->LoadMeshFromFile(csMeshfile, csTexturePath)))
		return E_FAIL;

	m_pMesh->GetBoundingB_S(&m_vOrigMax, &m_vOrigMin, &m_vCenter, &m_fRaius);

	m_vPos = D3DXVECTOR3(415,0,160);//Æ½ÒÆ
	m_vRot = D3DXVECTOR3(0,0,0);//Ðý×ª
	m_vScal = D3DXVECTOR3(30,30,30);//Ëõ·Å

	CTerrain *pTerrain = CDXDevice::Instance()->GetScene()->GetCurTerrain();
	m_vPos.y = pTerrain->GetTHeight(m_vPos.x, m_vPos.z);

	Transform();

	return S_OK;
}

void CDeck::Update()
{
	Transform();
}

void CDeck::Render()
{

	m_pDevice->SetRenderState(D3DRS_LIGHTING,false);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pMesh->Render();
//	m_pDevice->SetTransform(D3DTS_WORLD, &OldmatWorld);
	DrawLine(m_pDevice, m_vMin, m_vMax, 0x00ffffff);
}