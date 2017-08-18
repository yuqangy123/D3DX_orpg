#include "StdAfx.h"
#include "Mesh.h"

CMesh::CMesh(IDirect3DDevice9* p)
{
	m_pDevice = p;
	m_pMesh=NULL;
	m_dwNumMaterials=0;
	m_pMeshMaterials=NULL;
	m_pMeshTextures=NULL;
}

CMesh::~CMesh(void)
{
	SafeRelease(m_pMesh);
	DeleteArrayAndNull(m_pMeshMaterials);
	DeleteArrayAndNull(m_pMeshTextures);
}

void CMesh::Render()
{
	// ��Ⱦ�����
// 	DWORD AlphaText,AlphaBlend,CullMode, Lighting;
// 	m_pDevice->GetRenderState(D3DRS_CULLMODE,&CullMode);
// 	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE); 



	for (DWORD n=0; n<m_dwNumMaterials; ++n)
	{
		m_pDevice->SetMaterial(&m_pMeshMaterials[n]);
		m_pDevice->SetTexture(0, m_pMeshTextures[n]);
		m_pMesh->DrawSubset(n);
	}

//	m_pDevice->SetRenderState(D3DRS_CULLMODE,CullMode); 
}

HRESULT CMesh::LoadMeshFromFile(string_t MeshFile, string_t TextureFile)
{
	ID3DXBuffer* adjBuffer  = 0;//mesh���ڽ���Ϣ
	ID3DXBuffer* mtrlBuffer = 0;//mesh��������Ϣ
	if(FAILED(D3DXLoadMeshFromX(MeshFile.c_str(), 	D3DXMESH_MANAGED, m_pDevice, &adjBuffer, &mtrlBuffer, NULL, &m_dwNumMaterials,  &m_pMesh)))
		return E_FAIL;

	if(m_dwNumMaterials && mtrlBuffer)
	{
		// �Ӳ��ʼ����аѲ��ʺ���Ϣ��ѹ��ȡ����
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();//����һ��ָ��ʼ���ݵ�ָ��
		m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
		m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
		for (DWORD i=0; i<m_dwNumMaterials; i++)
		{
			// ��������
			m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
			// ���ò������������ɫ
			m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;
			// ��������
			char tmpTexture[256]={0};
			CStringA csTexturePath(TextureFile.c_str());
			sprintf_s(tmpTexture,"%s\\%s",csTexturePath.GetBuffer(), d3dxMaterials[i].pTextureFilename);
			if (FAILED(D3DXCreateTextureFromFileA(m_pDevice,tmpTexture,&m_pMeshTextures[i])))
			{
				m_pMeshTextures[i] = NULL;
			}
		}
		// �ͷŲ��ʻ�����������
		SafeRelease(mtrlBuffer);
		SafeRelease(adjBuffer);
	}

	return S_OK;
}

bool CMesh::GetBoundingB_S(D3DXVECTOR3* pMax,D3DXVECTOR3* pMin,D3DXVECTOR3* pCenter,float* pRadius)
{
	HRESULT hr = 0;
	D3DXVECTOR3 tmpMin,tmpMax;
	BYTE* pV;
	m_pMesh->LockVertexBuffer(0,(void**)&pV);
	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pV,                         // ָ���ڶ��������е�һ�����������
		m_pMesh->GetNumVertices(),	              // �ڶ��������еĵĶ�������
		D3DXGetFVFVertexSize(m_pMesh->GetFVF()),  // ÿ��������ֽڴ�С
		&tmpMin,								  // Χ�е���С��
		&tmpMax);								  // Χ�е�����
	m_pMesh->UnlockVertexBuffer();

	if (pMin != NULL)
		*pMin = tmpMin;

	if (pMax != NULL)
		*pMax = tmpMax;

	if (pCenter != NULL)
	{
		D3DXVECTOR3 tmpCenter;
		tmpCenter.x = (tmpMax.x - tmpMin.x) / 2;
		tmpCenter.y = (tmpMax.y - tmpMin.y) / 2;
		tmpCenter.z = (tmpMax.z - tmpMin.z) / 2;
		*pCenter = tmpCenter;
	}
	if (pRadius != NULL)
	{
		*pRadius = D3DXVec3Length(&(tmpMax - tmpMin));
	}

	if( FAILED(hr) )
		return false;

	return true;
}