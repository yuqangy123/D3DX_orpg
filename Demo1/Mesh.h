#pragma once
#include "common1.h"

class CMesh
{
public:
	CMesh(IDirect3DDevice9* p);
	~CMesh(void);
	HRESULT LoadMeshFromFile(string_t, string_t);
	void Render();
	bool GetBoundingB_S(D3DXVECTOR3* pMax,D3DXVECTOR3* pMin,D3DXVECTOR3* pCenter,float* pRadius);
	LPD3DXMESH GetD3DXMeshObj(){return m_pMesh;}
private:
	IDirect3DDevice9* m_pDevice;
	LPD3DXMESH		m_pMesh;
	DWORD		m_dwNumMaterials;
	D3DMATERIAL9		*m_pMeshMaterials;//≤ƒ÷ 
	LPDIRECT3DTEXTURE9		*m_pMeshTextures;//Œ∆¿Ì
};
