#pragma once
#include "MeshManager.h"
#include "ZFrustum.h"
#include "MajorObject.h"
#include "Camera.h"
#include "Terrain.h"
#include "MajorObject.h"
#include "Sky.h"
#include "Deck.h"
#include "ParticleSystem.h"

class CCamera;
class CMajorObject;
class CScene
{
public:
	CScene(IDirect3DDevice9* p);
	~CScene(void);
	HRESULT Init(CCamera *pCamera);
	void Render();
	void Update(double nFpsTime);
	CTerrain* GetCurTerrain();
	CDeck* GetDeck(){return m_pDeck;}
	//inline float GetTHeight(float x, float z);

private:
	CMeshManager		*m_pMeshManage;
	CTerrain			*m_pTerrain;
	CZFrustum		*m_pFrustum;
	CCamera		*m_pCamera;
	IDirect3DDevice9		*m_pDevice;
	ID3DXFont		*m_pFont;
	CMajorObject	*m_pMajor;
	CSky					*m_pSky;
	CDeck					*m_pDeck;
	CSnow	*m_pSnow;
};
