#include "StdAfx.h"
#include "Sky.h"
#include "DXDevice.h"
#include "Encoder.h"
CSky::CSky(void)
{
	m_pMesh = NULL;
}

CSky::~CSky(void)
{
	
	DeleteAndNull(m_pMesh);
}

HRESULT CSky::Init()
{
	m_pDevice = CDXDevice::Instance()->GetD3Device();
	m_pMesh = new CMesh(m_pDevice);
	astring_t meshfile = CResFileManager::Instance()->GetSkyMeshFile();
	astring_t texturefile = CResFileManager::Instance()->GetSkyTextureFile();
	string_t meshfileW, texturefileW;
	CEncoder Encoder;
	Encoder.SetMString(meshfile.c_str());
	meshfileW = Encoder.GetWString();
	Encoder.SetMString(texturefile.c_str());
	texturefileW = Encoder.GetWString();
	if(FAILED(m_pMesh->LoadMeshFromFile(meshfileW, texturefileW)))
		return E_FAIL;

	CResFileManager::Instance()->GetSkyAttribute(m_vPos, m_vRot, m_vScal);
	Transform();

	return S_OK;
}

void CSky::Update(D3DXVECTOR3 vPos)
{
	m_vPos = vPos;
	Transform();
}

void CSky::Render()
{
	DWORD OldLighting=0, OldZwriteEnable = 0, OldAlphablened = 0;
	//m_pDevice->GetTransform(D3DTS_WORLD, &OldmatWorld);
	m_pDevice->GetRenderState(D3DRS_LIGHTING, &OldLighting);
	m_pDevice->GetRenderState(D3DRS_ZWRITEENABLE,&OldZwriteEnable);
	m_pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &OldAlphablened);

	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pDevice->SetRenderState(D3DRS_LIGHTING,false);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);//打开深度缓冲区
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pMesh->Render();

	//m_pDevice->SetTransform(D3DTS_WORLD, &OldmatWorld);
	m_pDevice->SetRenderState(D3DRS_LIGHTING,OldLighting);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,OldZwriteEnable);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, OldAlphablened);
}
