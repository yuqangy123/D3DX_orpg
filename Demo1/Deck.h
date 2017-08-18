#pragma once
#include "Mesh.h"
#include "BaseObject.h"
#include "common1.h"

class CDeck : public CBaseObject
{
public:
	CDeck(void);
	~CDeck(void);
	HRESULT Init();
	void Update();
	void Render();
	LPD3DXBASEMESH GetD3DXMeshObj(){return m_pMesh->GetD3DXMeshObj();};

private:
	CMesh			*m_pMesh;
	IDirect3DDevice9		*m_pDevice;
};
