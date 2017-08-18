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
	DOUBLE					m_dElapsedTime;  // ÿ֡ʱ��
	float						m_fMoveSpeed;//�ƶ��ٶ�	
	D3DXVECTOR3					m_vDestWorld;
	float									m_fDistanceWithMajor;
	float									m_fMaxDistanceWithMajor;
	D3DXVECTOR3		m_vDir;//����
	D3DXVECTOR3		m_vOrigRot;//ģ��ԭʼ��ת����
	SkinMeshState		m_State;
	
};
