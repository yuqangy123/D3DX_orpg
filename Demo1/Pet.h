#pragma once
#include "BaseObject.h"
#include "SkinMesh.h"
#include "AnimInstance.h"
#include "Terrain.h"
#include "Input.h"
#include "Camera.h"


class CPet : public CBaseObject
{
public:
	CPet();
	~CPet(void);
	void Update(double nFpsTime, D3DXVECTOR3 vMajorPos);
	void Render();
	HRESULT Init();
	void InitPos(D3DXVECTOR3 vMajorPos, D3DXVECTOR3 vMajorDir);

protected:
	virtual void Transform();

private:
	IDirect3DDevice9				*m_pDevice;
	CTerrain								*m_pTerrain;
	CSkinMesh						*m_pSkinMesh;
	CAnimInstance					*m_pAI;
	DOUBLE					m_dElapsedTime;  // 每帧时间
	float						m_fMoveSpeed;//移动速度	
	D3DXVECTOR3					m_vDestWorld;
	float									m_fDistanceWithMajor;
	float									m_fMaxDistanceWithMajor;
	D3DXVECTOR3		m_vDir;//方向
	D3DXVECTOR3		m_vOrigRot;//模型原始旋转方向
	SkinMeshState		m_State;
	
};
