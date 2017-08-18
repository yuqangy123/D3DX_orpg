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
	// 渲染公告板
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
	ID3DXBuffer* adjBuffer  = 0;//mesh的邻接信息
	ID3DXBuffer* mtrlBuffer = 0;//mesh缓冲区信息
	if(FAILED(D3DXLoadMeshFromX(MeshFile.c_str(), 	D3DXMESH_MANAGED, m_pDevice, &adjBuffer, &mtrlBuffer, NULL, &m_dwNumMaterials,  &m_pMesh)))
		return E_FAIL;

	if(m_dwNumMaterials && mtrlBuffer)
	{
		// 从材质集合中把材质和信息解压读取出来
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();//返回一个指向开始数据的指针
		m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
		m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
		for (DWORD i=0; i<m_dwNumMaterials; i++)
		{
			// 拷贝材质
			m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
			// 设置材质漫反射的颜色
			m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;
			// 创建纹理
			char tmpTexture[256]={0};
			CStringA csTexturePath(TextureFile.c_str());
			sprintf_s(tmpTexture,"%s\\%s",csTexturePath.GetBuffer(), d3dxMaterials[i].pTextureFilename);
			if (FAILED(D3DXCreateTextureFromFileA(m_pDevice,tmpTexture,&m_pMeshTextures[i])))
			{
				m_pMeshTextures[i] = NULL;
			}
		}
		// 释放材质缓冲区的内容
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
		(D3DXVECTOR3*)pV,                         // 指向在顶点数组中第一个顶点的向量
		m_pMesh->GetNumVertices(),	              // 在顶点数组中的的顶点数。
		D3DXGetFVFVertexSize(m_pMesh->GetFVF()),  // 每个顶点的字节大小
		&tmpMin,								  // 围盒的最小点
		&tmpMax);								  // 围盒的最大点
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