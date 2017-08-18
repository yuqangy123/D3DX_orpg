#pragma once
#include "common1.h"
#include "BaseObject.h"
#include "mesh.h"

class CSky : public CBaseObject
{
public:
	CSky();
	~CSky(void);
	HRESULT Init();
	void Update(D3DXVECTOR3 vPos);
	void Render();

private:
	CMesh			*m_pMesh;	
	IDirect3DDevice9		*m_pDevice;
};
