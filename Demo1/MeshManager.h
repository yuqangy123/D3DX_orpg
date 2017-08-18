#pragma once
#include "common1.h"
#include "ResFileManager.h"
#include "Mesh.h"

class CMeshManager
{
public:
	CMeshManager(void);
	~CMeshManager(void);
	HRESULT LoadMeshFile();
	void Render();

private:
	vector_t<CMesh*>		*m_MeshList;
	IDirect3DDevice9			*m_pDevice;
};
