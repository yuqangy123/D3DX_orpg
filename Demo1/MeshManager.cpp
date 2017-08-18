#include "StdAfx.h"
#include "MeshManager.h"
#include "ResFileManager.h"
#include "DXDevice.h"

CMeshManager::CMeshManager(void)
{
	m_MeshList = NULL;
	m_MeshList = new vector_t<CMesh*>;
}

CMeshManager::~CMeshManager(void)
{
	DeleteAndNull(m_MeshList);
}

HRESULT CMeshManager::LoadMeshFile()
{
	m_pDevice = CDXDevice::Instance()->GetD3Device();
	m_MeshList->clear();
 	vector_t<string_t> FileList = CResFileManager::Instance()->GetMeshFile();
// 	for (vector_t<string_t>::iterator Itr = FileList.begin(); Itr != FileList.end(); ++Itr)
// 	{
// 		CMesh *pMesh = new CMesh(p);
// 		if(pMesh)
// 		{
// 			if(!FAILED(pMesh->LoadMeshFromFile(*Itr, CResFileManager::Instance()->GetTextureFilePath())))
// 				m_MeshList->push_back(pMesh);			
// 		}
// 	}

	return S_OK;
}

void CMeshManager::Render()
{
	for (vector_t<CMesh*>::iterator ConItr = m_MeshList->begin(); ConItr != m_MeshList->end(); ++ConItr)
	{
		(*ConItr)->Render();
	}
}